// standart library
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
// header-project
#include "../../../common/include/files.h"
#include "../include/create.h"

#define ERRNO_OK 0
#define MIN_ARGS 2
#define HELP_LEN 2
#define FAILED_MKDIR -1

static CreateState create_different_dir_r(ArgumentControlLer *pcontroller,
                                          char **argv, int argc);
static CreateState create_different_dir_p(ArgumentControlLer *pcontroller,
                                          char **argv, int argc);
static CreateState create_in_same_dir_helper(char *dirpath, char **argv,
                                             int argc);
static CreateState count_num_r(ArgumentControlLer *pcontroller);
static CreateState count_num_p(ArgumentControlLer *pcontroller);
static CreateState find_index_r(ArgumentControlLer *pcontroller, char *option,
                                char **argv, int argc);
static CreateState find_index_p(ArgumentControlLer *pcontroller, char **argv,
                                int argc);
CreateState extract_filename(char **filename, char *filepath,
                             size_t fpath_len) {

  if (filepath == NULL || fpath_len != strlen(filepath) ||
      fpath_len > PATH_MAX) {
    return CREATE_INVALID_FILEPATH;
  }

  size_t i = fpath_len - 1;
  while (filepath[i] != SEP_CHAR && i > 0) {
    --i;
  }

  if (filepath[i] == SEP_CHAR) {
    *filename = filepath + (i + 1);
  } else {
    *filename = filepath;
  }
  return CREATE_OK;
}

CreateState extract_directory_path(char **dirpath, char *copy_filepath,
                                   size_t filepath_len) {
  if (copy_filepath == NULL || filepath_len != strlen(copy_filepath)) {
    return CREATE_INVALID_FILEPATH;
  }

  size_t i = filepath_len - 1;
  while (copy_filepath[i] != SEP_CHAR && i > 0) {
    --i;
  }

  copy_filepath[i + 1] = END_STR;
  *dirpath = copy_filepath;
  return CREATE_OK;
}

CreateState validate_dirpath(const char *dirpath, size_t dirpath_len) {
  if (dirpath == NULL) {
    return CREATE_NULL_POINTER_EXCEPTION;
  }

  if (strlen(dirpath) != dirpath_len || dirpath_len > (PATH_MAX - 1) ||
      dirpath_len == 0) {
    return CREATE_INVALID_DIRPATH;
  }

  for (size_t i = 0; i < dirpath_len; ++i) {
    if (dirpath[i] == QUESTION_MARK || dirpath[i] == TILDE) {
      return CREATE_INVALID_DIRPATH;
    }
  }

  return CREATE_OK;
}

CreateState create_file(const char *filepath, size_t fpath_len) {
  if (filepath == NULL) {
    return CREATE_NULL_POINTER_EXCEPTION;
  }

  if (fpath_len == 0) {
    return CREATE_INVALID_FILEPATH;
  }

  char *filename = NULL;

  char *copy_filepath = (char *)malloc(sizeof(filepath[0]) * (fpath_len + 1));
  if (copy_filepath == NULL) {
    return CREATE_UNKNOWN_ERROR;
  }

  strncpy(copy_filepath, filepath, fpath_len);

  copy_filepath[fpath_len] = END_STR;

  CreateState res = extract_filename(&filename, copy_filepath, fpath_len);
  if (res != CREATE_OK) {
    free(copy_filepath);
    return res;
  }

  if (filename == NULL) {
    free(copy_filepath);
    return CREATE_INVALID_FILENAME;
  }

  res = validate_filename(filename, strlen(filename));

  if (res != CREATE_OK) {
    free(copy_filepath);
    return res;
  }

  char *dirpath = NULL;

  res = extract_directory_path(&dirpath, copy_filepath, fpath_len);
  if (res != CREATE_OK) {
    free(copy_filepath);
    return res;
  }

  if (dirpath == NULL) {
    free(copy_filepath);
    return CREATE_EXTRACT_DIRPATH_FAILED;
  }

  res = validate_dirpath(dirpath, strlen(dirpath));

  if (res != CREATE_OK) {
    free(copy_filepath);
    copy_filepath = NULL;
    return res;
  }

#ifdef CDF
  res = create_directory(dirpath, strlen(dirpath));
  if (res != CREATE_OK) {
    free(copy_filepath);
    return res;
  }
#endif

  mode_t mode = 0644;

  errno = ERRNO_OK;

  int fd = open(filepath, O_CREAT | O_EXCL | O_WRONLY | O_NOFOLLOW | O_CLOEXEC,
                mode);
  switch (errno) {
  case EEXIST:
    free(copy_filepath);
    return CREATE_FILE_ALREADY_EXISTS;
  case ENOENT:
    free(copy_filepath);
    return CREATE_INVALID_DIRPATH;
  case ERRNO_OK:
    break;
  default:
    free(copy_filepath);
    return CREATE_UNKNOWN_ERROR;
  }
  close(fd);

  free(copy_filepath);

  return CREATE_OK;
}

CreateState choose_option(char *option, char **argv, int argc) {
  if (option == NULL || argv == NULL) {
    return CREATE_NULL_POINTER_EXCEPTION;
  }

  AvaibleOption avaible_options[] = {{DF_OPTION, DIFFERENT_FILEPATH},
                                     {SD_OPTION, SAME_DIR},
                                     {DD_OPTION, DIFFERENT_DIRS},
                                     {HELP_OPTION, HELP},
                                     {PD_OPTION, PARENT}};

  const size_t avaible_options_len =
      sizeof(avaible_options) / sizeof(avaible_options[0]);

  OptionIndex option_index = INVALID_OPTION;

  for (size_t i = 0; i < avaible_options_len; ++i) {
    if (strlen(option) == strlen(avaible_options[i].option) &&
        strcmp(avaible_options[i].option, option) == 0) {
      option_index = avaible_options[i].option_index;
    }
  }

  switch (option_index) {
  case DIFFERENT_FILEPATH:
    return argc > MIN_ARGS
               ? create_different_filepath(argv + MIN_ARGS, argc - MIN_ARGS)
               : CREATE_INVALID_OPTION;
  case SAME_DIR:
    return argc > MIN_ARGS ? create_in_same_dir(argv, argc)
                           : CREATE_INVALID_OPTION;
  case DIFFERENT_DIRS:
    return argc > MIN_ARGS ? create_different_dir(argv, argc)
                           : CREATE_INVALID_OPTION;
  case HELP:
    return argc == HELP_LEN ? CREATE_HELP : CREATE_INVALID_OPTION;
#ifdef CDF
  case PARENT:
    return argc > MIN_ARGS ? create_different_dir(argv, argc)
                           : CREATE_INVALID_OPTION;
#endif
  default:
    return CREATE_INVALID_OPTION;
  }
}

CreateState create_directory(const char *dirpath, size_t dirpath_len) {

  CreateState state = validate_dirpath(dirpath, dirpath_len);
  if (state != CREATE_OK) {
    return state;
  }

  char *copy_dirpath = (char *)malloc(sizeof(char) * (dirpath_len + 1));
  if (copy_dirpath == NULL) {
    return CREATE_NULL_POINTER_EXCEPTION;
  }

  snprintf(copy_dirpath, (dirpath_len + 1), "%s", dirpath);
  mode_t mode = 0755;

  for (size_t i = 0; i < dirpath_len; ++i) {
    if (copy_dirpath[i] == SEP_CHAR && i != 0) {
      errno = ERRNO_OK;
      copy_dirpath[i] = END_STR;

      if (mkdir(copy_dirpath, mode) == FAILED_MKDIR && errno != EEXIST) {
        free(copy_dirpath);
        return CREATE_FAILED_DIR_CREATION;
      }
      copy_dirpath[i] = SEP_CHAR;
    }
  }

  free(copy_dirpath);
  return CREATE_OK;
}

CreateState create_in_same_dir(char **argv, int argc) {
  if (argv == NULL || argc == 0) {
    return CREATE_INVALID_ARGUMENT;
  }

  ArgumentControlLer controller = {NULL, 0, 0, NULL, 0, 0, NULL, 0, 0};

  CreateState state = find_index(&controller, SD_OPTION, argv, argc);
  if (state != CREATE_OK) {
    return state;
  }

  state = count_num(&controller);
  if (state != CREATE_OK) {
    return state;
  }

  state = create_different_dir_r(&controller, argv, argc);
  if (state != CREATE_OK) {
    return state;
  }

  return CREATE_OK;
}

CreateState create_different_dir(char **argv, int argc) {
  ArgumentControlLer controller = {NULL, 0, 0, NULL, 0, 0, NULL, 0, 0};
  if (argc > MIN_ARGS && strcmp(argv[MIN_ARGS - 1], PD_OPTION) == 0) {
    for (int i = 0; i < argc;
         i = controller.fmarker_i + controller.files_num + 1) {

      CreateState state = find_index(&controller, PD_OPTION, argv, argc);
      if (state != CREATE_OK) {
        return state;
      }
      state = count_num(&controller);
      if (state != CREATE_OK) {
        return state;
      }

      state = create_different_dir_p(&controller, argv, argc);
      if (state != CREATE_OK) {
        return state;
      }
    }
  } else {
    for (int i = 0; i < argc;
         i = controller.fmarker_i + controller.files_num + 1) {

      CreateState state = find_index(&controller, DD_OPTION, argv, argc);
      if (state != CREATE_OK) {
        return state;
      }

      state = count_num(&controller);
      if (state != CREATE_OK) {
        return state;
      }

      state = create_different_dir_r(&controller, argv, argc);
      if (state != CREATE_OK) {
        return state;
      }
    }
  }

  return CREATE_OK;
}

static CreateState create_different_dir_p(ArgumentControlLer *pcontroller,
                                          char **argv, int argc) {
  if (argv == NULL || argc == 0) {
    return CREATE_INVALID_ARGUMENT;
  }

  for (int i = 1; i <= pcontroller->parent_num; ++i) {
    for (int j = 1; j <= pcontroller->child_num; ++j) {
      int dirpath_len = strlen((pcontroller->parent_marker)[i]) + 1 +
                        strlen((pcontroller->child_marker)[j]) + 1;
      char *dirpath = (char *)malloc(dirpath_len + 1);
      if (dirpath == NULL) {
        return CREATE_NULL_POINTER_EXCEPTION;
      }
      snprintf(dirpath, dirpath_len + 1, "%s/%s/",
               pcontroller->parent_marker[i], pcontroller->child_marker[j]);
      CreateState state = create_in_same_dir_helper(
          dirpath, pcontroller->files_marker + 1, pcontroller->files_num);
      if (state != CREATE_OK) {
        free(dirpath);
        return state;
      }
      free(dirpath);
    }
  }
  return CREATE_OK;
}

static CreateState create_different_dir_r(ArgumentControlLer *pcontroller,
                                          char **argv, int argc) {
  if (argv == NULL || argc == 0) {
    return CREATE_INVALID_ARGUMENT;
  }

  for (int i = 1; i <= pcontroller->parent_num; ++i) {
    CreateState state = create_in_same_dir_helper(pcontroller->parent_marker[i],
                                                  pcontroller->files_marker + 1,
                                                  pcontroller->files_num);
    if (state != CREATE_OK) {
      return state;
    }
  }
  return CREATE_OK;
}

CreateState create_different_filepath(char **argv, int argc) {
  CreateState state = CREATE_OK;
  for (int i = 0; i < argc; ++i) {
    state = create_file(argv[i], strlen(argv[i]));
    if (state != CREATE_OK) {
      for (int j = i; j < argc; ++j) {
        fprintf(stderr, "fails to create %s\n", argv[j]);
      }
      return state;
    }
  }

  return state;
}

CreateState find_index(ArgumentControlLer *pcontroller, char *option,
                       char **argv, int argc) {

  if (pcontroller == NULL || option == NULL || argv == NULL) {
    return CREATE_NULL_POINTER_EXCEPTION;
  }

  if (argc <= 0) {
    return CREATE_INVALID_ARGC;
  }

  pcontroller->parent_marker = NULL;
  pcontroller->child_marker = NULL;
  pcontroller->files_marker = NULL;

  pcontroller->argc = argc;

  return strncmp(option, PD_OPTION, strlen(option) + 1) == 0
             ? find_index_p(pcontroller, argv, argc)
             : find_index_r(pcontroller, option, argv, argc);
}

static CreateState find_index_p(ArgumentControlLer *pcontroller, char **argv,
                                int argc) {

  int i = pcontroller->fmarker_i + 1;

  while (i < argc && strncmp(argv[i], FILE_MARKER, strlen(argv[i]) + 1) != 0) {
    if (strncmp(argv[i], PD_OPTION, strlen(argv[i]) + 1) == 0) {
      pcontroller->parent_marker = argv + i;
      pcontroller->pmarker_i = i;
    } else if (strncmp(argv[i], CHILD_MARKER, strlen(argv[i])) == 0) {
      pcontroller->child_marker = argv + i;
      pcontroller->child_i = i;
    }
    ++i;
  }

  if (i < argc && pcontroller->pmarker_i < pcontroller->child_i &&
      pcontroller->child_i < i) {
    pcontroller->files_marker = argv + i;
    pcontroller->fmarker_i = i;
    return CREATE_OK;
  }

  return CREATE_INVALID_ARGUMENT;
}

static CreateState find_index_r(ArgumentControlLer *pcontroller, char *option,
                                char **argv, int argc) {
  int i = pcontroller->fmarker_i + 1;

  while (i < argc && strncmp(argv[i], FILE_MARKER, strlen(argv[i]) + 1) != 0) {
    if (strncmp(argv[i], option, strlen(argv[i]) + 1) == 0) {
      pcontroller->parent_marker = argv + i;
      pcontroller->pmarker_i = i;
    }
    ++i;
  }

  if (i < argc && pcontroller->pmarker_i < i) {
    pcontroller->files_marker = argv + i;
    pcontroller->fmarker_i = i;
    return CREATE_OK;
  }
  return CREATE_INVALID_ARGUMENT;
}
CreateState count_num(ArgumentControlLer *pcontroller) {
  if (pcontroller == NULL) {
    return CREATE_NULL_POINTER_EXCEPTION;
  }

  if (pcontroller->parent_marker == NULL || pcontroller->files_marker == NULL) {
    return CREATE_INVALID_ARGUMENT;
  }

  pcontroller->parent_num = 0;
  pcontroller->child_num = 0;
  pcontroller->files_num = 0;

  bool is_pdoption = strncmp(pcontroller->parent_marker[0], PD_OPTION,
                             strlen(PD_OPTION) + 1) == 0;

  if (is_pdoption && pcontroller->child_marker == NULL) {
    return CREATE_INVALID_ARGUMENT;
  }

  return is_pdoption ? count_num_p(pcontroller) : count_num_r(pcontroller);
}

static CreateState count_num_p(ArgumentControlLer *pcontroller) {
  if (pcontroller == NULL || pcontroller->parent_marker == NULL) {
    return CREATE_INVALID_ARGUMENT;
  }
  char **ptr = pcontroller->parent_marker + 1;

  while (*ptr != NULL &&
         strncmp(*ptr, pcontroller->parent_marker[0], strlen(*ptr) + 1) != 0 &&
         (pcontroller->fmarker_i + pcontroller->files_num) <
             pcontroller->argc) {
    if (strncmp(*ptr, pcontroller->parent_marker[0], strlen(*ptr) + 1) == 0 &&
        ptr != pcontroller->parent_marker) {
      return CREATE_INVALID_ARGC;
    } else if (strncmp(*ptr, pcontroller->child_marker[0], strlen(*ptr) + 1) ==
                   0 &&
               ptr != pcontroller->child_marker) {
      return CREATE_INVALID_ARGC;
    } else if (strncmp(*ptr, pcontroller->files_marker[0], strlen(*ptr) + 1) ==
                   0 &&
               ptr != pcontroller->files_marker) {
      return CREATE_INVALID_ARGC;
    }
    if (ptr < pcontroller->child_marker) {
      ++pcontroller->parent_num;
    } else if (ptr > pcontroller->child_marker &&
               ptr < pcontroller->files_marker) {
      ++pcontroller->child_num;
    } else if (ptr > pcontroller->files_marker &&
               strncmp(*ptr, FILE_MARKER, strlen(*ptr) + 1) != 0) {
      ++pcontroller->files_num;
    }
    ++ptr;
  }

  return pcontroller->parent_num == 0 || pcontroller->child_num == 0 ||
                 pcontroller->files_num == 0
             ? CREATE_INVALID_ARGC
             : CREATE_OK;
}

static CreateState count_num_r(ArgumentControlLer *pcontroller) {
  if (pcontroller == NULL || pcontroller->parent_marker == NULL) {
    return CREATE_INVALID_ARGUMENT;
  }
  char **ptr = pcontroller->parent_marker + 1;

  while (*ptr != NULL &&
         strncmp(*ptr, pcontroller->parent_marker[0], strlen(*ptr) + 1) != 0 &&
         (pcontroller->fmarker_i + pcontroller->files_num) <
             pcontroller->argc) {
    if (strncmp(*ptr, pcontroller->parent_marker[0], strlen(*ptr) + 1) == 0 &&
        ptr != pcontroller->parent_marker) {
      return CREATE_INVALID_ARGC;
    } else if (strncmp(*ptr, CHILD_MARKER, strlen(*ptr) + 1) == 0) {
      return CREATE_INVALID_ARGC;
    } else if (strncmp(*ptr, pcontroller->files_marker[0], strlen(*ptr) + 1) ==
                   0 &&
               ptr != pcontroller->files_marker) {
      return CREATE_INVALID_ARGC;
    }

    if (ptr < pcontroller->files_marker) {
      ++pcontroller->parent_num;
    } else if (ptr > pcontroller->files_marker &&
               strncmp(*ptr, FILE_MARKER, strlen(*ptr) + 1) != 0) {
      ++pcontroller->files_num;
    }
    ++ptr;
  }

  return pcontroller->parent_num == 0 || pcontroller->files_num == 0
             ? CREATE_INVALID_ARGC
             : CREATE_OK;
}

static CreateState create_in_same_dir_helper(char *dirpath, char **argv,
                                             int argc) {
  if (dirpath == NULL) {
    return CREATE_INVALID_DIRPATH;
  }

  if (argv == NULL || argc == 0) {
    return CREATE_INVALID_FILEPATH;
  }

  CreateState state = validate_dirpath(dirpath, strlen(dirpath));
  if (state != CREATE_OK) {
    return CREATE_INVALID_DIRPATH;
  }

  const int fail_to_print = -1;

  for (int i = 0; i < argc; ++i) {
    size_t default_len = strlen(dirpath) + strlen(argv[i]);
    size_t fpath_len = dirpath[strlen(dirpath) - 1] == SEP_CHAR
                           ? default_len
                           : default_len + 1;
    char *filepath = (char *)malloc(sizeof(dirpath[0]) * (fpath_len + 1));
    if (filepath == NULL) {
      return CREATE_NULL_POINTER_EXCEPTION;
    }

    if (fpath_len == default_len &&
        snprintf(filepath, (fpath_len + 1), "%s%s", dirpath, argv[i]) ==
            fail_to_print) {
      free(filepath);
      return CREATE_FAILED_FILEPATH;
    } else if (fpath_len == (default_len + 1) &&
               snprintf(filepath, (fpath_len + 1), "%s/%s", dirpath, argv[i]) ==
                   fail_to_print) {
      free(filepath);
      return CREATE_FAILED_FILEPATH;
    }

    state = create_file(filepath, fpath_len);

    if (state != CREATE_OK) {
      free(filepath);
      return state;
    }
    free(filepath);
    filepath = NULL;
  }

  return state;
}
