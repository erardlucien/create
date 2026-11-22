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

static CreateState create_different_dir_helper(char *dirpath, char **argv,
                                               int argc);
static CreateState create_in_same_dir_helper(char *dirpath, char **argv,
                                             int argc);
static CreateState count_elem(int *pcounter, char *start_deliminator,
                              int *pindex_st_deliminator, char *end_deliminator,
                              char **argv, int argc);
static CreateState gen_list(int counter, char ***list, int index_st_deliminator,
                            char **argv, int argc);

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

  if (strlen(dirpath) != dirpath_len || dirpath_len > (PATH_MAX - 1)) {
    return CREATE_INVALID_DIRPATH;
  }

  if (dirpath[dirpath_len - 1] != SEP_CHAR) {
    return CREATE_INVALID_DIRPATH;
  }

  if (dirpath_len == strlen(HOMEUSER_DIR) &&
      strcmp(dirpath, HOMEUSER_DIR) == 0) {
    return CREATE_OK;
  }

  if (dirpath_len == strlen(ACTUAL_DIR) && strcmp(dirpath, ACTUAL_DIR) == 0) {
    return CREATE_OK;
  }

  if (dirpath_len == strlen(PARENT_DIR) && strcmp(dirpath, PARENT_DIR) == 0) {
    return CREATE_OK;
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

  mode_t mode = 0644;

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

  AvaibleOption avaible_options[] = {{DD_OPTION, DIFFERENT_FILEPATH},
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
  count_elem(&dircounter, SD_OPTION, &dirst_delim_i, FILE_MARKER, argv, argc);
  count_elem(&filecounter, FILE_MARKER, &filest_delim_i, "", argv, argc);

  gen_list(dircounter, &dirlist, dirst_delim_i, argv, argc);
  gen_list(filecounter, &filelist, filest_delim_i, argv, argc);

  for (int i = 0; i < dircounter; ++i) {
    CreateState state =
        create_in_same_dir_helper(dirlist[i], filelist, filecounter);
    if (state != CREATE_OK) {
      for (int j = 0; j < filecounter; ++j) {
        fprintf(stderr, "cnf fails to create %s in %s\n", filelist[j],
                dirlist[i]);
      }
      free(dirlist);
      free(filelist);
      return state;
    }
  }

  free(dirlist);
  free(filelist);
  return CREATE_OK;
}

CreateState create_different_dir(char **argv, int argc) {
  if (argv == NULL || argc == 0) {
    return CREATE_UNKNOWN_ERROR;
  }

  char **dirlist = NULL;
  char **filelist = NULL;
  int dircounter = 0;
  int dirst_delim_i = 0;
  int filecounter = 0;
  int filest_delim_i = 0;
  for (int i = 0; i < argc; ++i) {
    count_elem(&dircounter, DD_OPTION, &dirst_delim_i, FILE_MARKER, argv, argc);
    count_elem(&filecounter, FILE_MARKER, &filest_delim_i, DD_OPTION, argv,
               argc);

    gen_list(dircounter, &dirlist, dirst_delim_i, argv, argc);
    gen_list(filecounter, &filelist, filest_delim_i, argv, argc);

    for (int j = 0; j < dircounter; ++j) {
      CreateState state =
          create_different_dir_helper(dirlist[j], filelist, filecounter);
      if (state != CREATE_OK) {
        for (int k = 0; k < filecounter; ++k) {
          fprintf(stderr, "cnf fails to create %s in %s\n", filelist[k],
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

static CreateState gen_list(int counter, char ***list, int index_st_deliminator,
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
static CreateState count_elem(int *pcounter, char *start_deliminator,
                              int *pindex_st_deliminator, char *end_deliminator,
                              char **argv, int argc) {
  if (start_deliminator == NULL || argc == 0) {
    return CREATE_UNKNOWN_ERROR;
  }

  int i = (*pindex_st_deliminator) + (*pcounter);
  while (i < argc &&
         strncmp(argv[i], start_deliminator, strlen(argv[i])) != 0) {
    ++i;
  }
  *pindex_st_deliminator = i;
  ++i;
  while (i < argc && strncmp(argv[i], end_deliminator, strlen(argv[i])) != 0) {
    ++(*pcounter);
    ++i;
  }

  if (i == *pindex_st_deliminator) {
    return CREATE_UNKNOWN_ERROR;
  }

  return CREATE_OK;
}

static CreateState create_different_dir_helper(char *dirpath, char **argv,
                                               int argc) {
  CreateState state = CREATE_OK;
  char **copy_argv = (char **)malloc(sizeof(char *) * argc);
  if (copy_argv == NULL) {
    return CREATE_UNKNOWN_ERROR;
  }

  for (int i = 0; i < argc; ++i) {
    size_t arg_len = strlen(dirpath) + strlen(argv[i]);
    copy_argv[i] = (char *)malloc(sizeof(char) * (arg_len + 1));
    if (copy_argv[i] == NULL) {
      free_vector(&copy_argv, i);
      return CREATE_UNKNOWN_ERROR;
    }
    snprintf(copy_argv[i], (arg_len + 1), "%s", argv[i]);
    int list_len = 0;
    const int count_args_fails = -1;
    if (count_args(&list_len, copy_argv[i], arg_len) == count_args_fails) {
      free_vector(&copy_argv, (i + 1));
      return CREATE_UNKNOWN_ERROR;
    }

    char **list = (char **)malloc(sizeof(char *) * list_len);
    if (list == NULL) {
      free_vector(&copy_argv, (i + 1));
      return CREATE_UNKNOWN_ERROR;
    }

    if (!extract_list(list_len, &list, copy_argv[i], arg_len)) {
      free(list);
      free_vector(&copy_argv, (i + 1));
      return CREATE_INVALID_ARGUMENT;
    }

    state = create_in_same_dir_helper(dirpath, list, list_len);
    if (state != CREATE_OK) {
      free(list);
      free_vector(&copy_argv, (i + 1));
      return state;
    }
    free(list);
  }
  free_vector(&copy_argv, argc);

  return state;
}

CreateState create_different_filepath(char **argv, int argc) {
  CreateState state = CREATE_OK;
  for (int i = 0; i < argc; ++i) {
    state = create_file(argv[i], strlen(argv[i]));
    if (state != CREATE_OK) {
      for (int j = i; j < argc; ++j) {
        fprintf(stderr, "cnf fails to create %s", argv[i]);
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

bool is_arg_valid(const char *argument, size_t argument_len) {
  for (size_t i = 0; i < argument_len; ++i) {
    if (argument[i] == FILE_DELIMITATOR) {
      return true;
    }
  }
  return false;
}

bool extract_list(int list_len, char ***list, char *argument,
                  size_t argument_len) {
  if (list == NULL || *list == NULL || list_len <= 0 || argument_len <= 0 ||
      !is_arg_valid(argument, argument_len)) {
    return false;
  }

  int count = 0;

  for (size_t i = 0; i < argument_len && count < list_len; ++i) {
    if (argument[i] == FILE_DELIMITATOR && (i + 1) < argument_len) {
      *(*(list) + count) = argument + (i + 1);
      argument[i] = END_STR;
      ++count;
    }
  }

  return true;
}

int count_args(int *count, char *argument, size_t argument_len) {
  int res = -1;

  if (count == NULL || argument == NULL || argument_len == 0) {
    return res;
  }

  for (size_t i = 0; i < argument_len; ++i) {
    if (argument[i] == FILE_DELIMITATOR) {
      ++(*count);
    }
  }

  res = 0;

  return res;
}
