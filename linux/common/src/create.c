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

static CreateState create_different_dir_r(char **argv, int argc);
static CreateState create_different_dir_p(char **argv, int argc);
static CreateState create_in_same_dir_helper(char *dirpath, char **argv,
                                             int argc);
CreateState extract_filename(char **filename, char *filepath,
                             size_t fpath_len) {

  if (filepath == NULL || fpath_len != strlen(filepath) ||
      fpath_len > PATH_MAX) {
    return CREATE_INVALID_FILEPATH;
  }

  if (filepath[fpath_len - 1] == SEP_CHAR) {
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

  if (dirpath[dirpath_len - 1] != SEP_CHAR) {
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
                                     {HELP_OPTION, HELP}};

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
      if (strcmp(copy_dirpath, getenv("HOME")) != 0 &&
          mkdir(copy_dirpath, mode) == FAILED_MKDIR && errno != EEXIST) {
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
    return CREATE_UNKNOWN_ERROR;
  }
  char **dirlist = NULL;
  char **filelist = NULL;
  int dircounter = 0;
  int dirst_delim_i = 0;
  int filecounter = 0;
  int filest_delim_i = 0;
  CreateState state = count_elem(&dircounter, SD_OPTION, &dirst_delim_i,
                                 FILE_MARKER, argv, argc);
  if (state != CREATE_OK) {
    return state;
  }
  state =
      count_elem(&filecounter, FILE_MARKER, &filest_delim_i, "", argv, argc);
  if (state != CREATE_OK) {
    return state;
  }

  if (dircounter == 0 || filecounter == 0) {
    return CREATE_UNKNOWN_ERROR;
  }

  state = gen_list(dircounter, &dirlist, dirst_delim_i, argv, argc);
  if (state != CREATE_OK) {
    return state;
  }
  state = gen_list(filecounter, &filelist, filest_delim_i, argv, argc);
  if (state != CREATE_OK) {
    free(dirlist);
    return state;
  }

  for (int i = 0; i < dircounter; ++i) {
    CreateState state =
        create_in_same_dir_helper(dirlist[i], filelist, filecounter);
    if (state != CREATE_OK) {
      for (int j = 0; j < filecounter; ++j) {
        fprintf(stderr, "fails to create %s in %s\n", filelist[j], dirlist[i]);
      }
      free(dirlist);
      free(filelist);
      return state;
    }
  }

  free(dirlist);
  free(filelist);
  return state;
}

CreateState create_different_dir(char **argv, int argc) {
  return argc > (MIN_ARGS + 1) && strcmp(argv[MIN_ARGS], PARENT_MARKER) == 0
             ? create_different_dir_p(argv, argc)
             : create_different_dir_r(argv, argc);
}

static CreateState create_different_dir_p(char **argv, int argc) {
  if (argv == NULL || argc == 0) {
    return CREATE_UNKNOWN_ERROR;
  }

  if (argc < (MIN_ARGS + 1)) {
    return CREATE_INVALID_ARGC;
  }

  char **parentlist = NULL;
  char **dirlist = NULL;
  char **filelist = NULL;
  int parentcounter = 0;
  int parentst_delim_i = 0;
  int dircounter = 0;
  int dirst_delim_i = 0;
  int filecounter = 0;
  int filest_delim_i = 0;
  for (int i = 0; i < argc && !(parentst_delim_i >= (argc - 1) ||
                                dirst_delim_i >= (argc - 1) ||
                                filest_delim_i >= (argc - 1));
       ++i) {

    CreateState state = count_elem(&parentcounter, PARENT_MARKER,
                                   &parentst_delim_i, CHILD_MARKER, argv, argc);
    if (state != CREATE_OK) {
      return state;
    }

    state = count_elem(&dircounter, CHILD_MARKER, &dirst_delim_i, FILE_MARKER,
                       argv, argc);
    if (state != CREATE_OK) {
      return state;
    }
    state = count_elem(&filecounter, FILE_MARKER, &filest_delim_i, DD_OPTION,
                       argv, argc);
    if (state != CREATE_OK) {
      return state;
    }

    if (dircounter == 0 || filecounter == 0 || parentcounter == 0) {
      return CREATE_UNKNOWN_ERROR;
    }

    state = gen_list(parentcounter, &parentlist, parentst_delim_i, argv, argc);
    if (state != CREATE_OK) {
      return state;
    }

    state = gen_list(dircounter, &dirlist, dirst_delim_i, argv, argc);
    if (state != CREATE_OK) {
      free(parentlist);
      return state;
    }

    state = gen_list(filecounter, &filelist, filest_delim_i, argv, argc);
    if (state != CREATE_OK) {
      free(parentlist);
      free(dirlist);
      return state;
    }

    for (int j = 0; j < parentcounter; ++j) {
      for (int k = 0; k < dircounter; ++k) {
        size_t dirpath_len = strlen(parentlist[j]) + strlen(dirlist[k]) + 1;
        char *dirpath = (char *)malloc(dirpath_len + 1);
        snprintf(dirpath, (dirpath_len + 1), "%s/%s/", parentlist[j],
                 dirlist[k]);
        state = create_in_same_dir_helper(dirpath, filelist, filecounter);
        if (state != CREATE_OK) {
          for (int l = 0; l < filecounter; ++l) {
            fprintf(stderr, "fails to create %s in %s\n", filelist[l], dirpath);
          }
          free(dirpath);
          free(parentlist);
          free(dirlist);
          free(filelist);
          return state;
        }
        free(dirpath);
      }
    }
    parentst_delim_i += parentcounter;
    dirst_delim_i += dircounter;
    filest_delim_i += filecounter;
    parentcounter = 0;
    dircounter = 0;
    filecounter = 0;

    free(parentlist);
    free(dirlist);
    free(filelist);
    parentlist = NULL;
    dirlist = NULL;
    filelist = NULL;
  }

  return CREATE_OK;
}

static CreateState create_different_dir_r(char **argv, int argc) {
  if (argv == NULL || argc == 0) {
    return CREATE_UNKNOWN_ERROR;
  }

  char **dirlist = NULL;
  char **filelist = NULL;
  int dircounter = 0;
  int dirst_delim_i = 0;
  int filecounter = 0;
  int filest_delim_i = 0;
  for (int i = 0; i < argc && !(dirst_delim_i >= (argc - 1) ||
                                filest_delim_i >= (argc - 1));
       ++i) {
    CreateState state = count_elem(&dircounter, DD_OPTION, &dirst_delim_i,
                                   FILE_MARKER, argv, argc);
    if (state != CREATE_OK) {
      return state;
    }
    state = count_elem(&filecounter, FILE_MARKER, &filest_delim_i, DD_OPTION,
                       argv, argc);
    if (state != CREATE_OK) {
      return state;
    }

    if (dircounter == 0 || filecounter == 0) {
      return CREATE_UNKNOWN_ERROR;
    }

    state = gen_list(dircounter, &dirlist, dirst_delim_i, argv, argc);
    if (state != CREATE_OK) {
      return state;
    }

    state = gen_list(filecounter, &filelist, filest_delim_i, argv, argc);
    if (state != CREATE_OK) {
      free(dirlist);
      return state;
    }

    for (int j = 0; j < dircounter; ++j) {
      state = create_in_same_dir_helper(dirlist[j], filelist, filecounter);
      if (state != CREATE_OK) {
        for (int k = 0; k < filecounter; ++k) {
          fprintf(stderr, "fails to create %s in %s\n", filelist[k],
                  dirlist[j]);
        }
        free(dirlist);
        free(filelist);
        return state;
      }
    }
    dirst_delim_i += dircounter;
    filest_delim_i += filecounter;
    dircounter = 0;
    filecounter = 0;

    free(dirlist);
    free(filelist);
    dirlist = NULL;
    filelist = NULL;
  }

  return CREATE_OK;
}

CreateState gen_list(int counter, char ***list, int index_st_deliminator,
                     char **argv, int argc) {
  if (counter <= 0) {
    return CREATE_UNKNOWN_ERROR;
  }

  *list = (char **)malloc(sizeof(char *) * (size_t)counter);
  if (*list == NULL) {
    return CREATE_UNKNOWN_ERROR;
  }
  int j = 0;
  for (int i = index_st_deliminator + 1; i < argc && j < counter; ++i) {
    *(*(list) + j) = argv[i];
    ++j;
  }
  return CREATE_OK;
}

CreateState count_elem(int *pcounter, char *start_deliminator,
                       int *pindex_st_deliminator, char *end_deliminator,
                       char **argv, int argc) {
  if (start_deliminator == NULL || end_deliminator == NULL || argc == 0) {
    return CREATE_UNKNOWN_ERROR;
  }

  int i = (*pindex_st_deliminator) + (*pcounter);
  while (i < argc &&
         strncmp(argv[i], start_deliminator, strlen(argv[i])) != 0) {
    ++i;
  }
  *pindex_st_deliminator = i;
  (*pcounter) = 0;
  ++i;
  while (i < argc && strncmp(argv[i], end_deliminator, strlen(argv[i])) != 0) {
    ++(*pcounter);
    ++i;
  }

  if ((*pcounter) == 0) {
    return CREATE_UNKNOWN_ERROR;
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
    size_t fpath_len = strlen(dirpath) + strlen(argv[i]);
    char *filepath = (char *)malloc(sizeof(dirpath[0]) * (fpath_len + 1));
    if (filepath == NULL) {
      return CREATE_UNKNOWN_ERROR;
    }

    if (snprintf(filepath, (fpath_len + 1), "%s%s", dirpath, argv[i]) ==
        fail_to_print) {
      free(filepath);
      return CREATE_UNKNOWN_ERROR;
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

void free_vector(char ***pvector, int n_row) {
  if (pvector == NULL || *pvector == NULL) {
    return;
  }
  for (int i = 0; i < n_row; ++i) {
    free(*(*(pvector) + i));
  }
  free((*pvector));
  *pvector = NULL;
}
