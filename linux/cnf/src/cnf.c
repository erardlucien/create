// standart library
#include <asm-generic/errno-base.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/limits.h>
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
#include "../include/cnf.h"

#define SEP_STR "/"
#define SEP_CHAR '/'
#define TWO_POINTS ".."
#define POINT '.'
#define STR_POINT "."
#define HYPHEN '-'
#define UNDERSCORE '_'
#define FILE_DELIMITATOR ':'

static bool is_dir_exist(const char *dirpath);
static int count_args(int *count, char *argument, size_t argument_len);
static bool is_arg_valid(const char *argument, size_t argument_len);
static bool extract_list(int list_len, char ***list, char *argument,
                         size_t argument_len);

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
  if (i != 0) {
    copy_filepath[i + 1] = END_STR;
    *dirpath = copy_filepath;
    return CREATE_OK;
  }
  return CREATE_NO_DIRPATH;
}

CreateState validate_dirpath(const char *dirpath, size_t dirpath_len) {
  if (dirpath == NULL || strlen(dirpath) != dirpath_len ||
      dirpath_len > PATH_MAX) {
    return CREATE_INVALID_DIRPATH;
  }

  if (dirpath_len == strlen(STR_POINT) && strcmp(dirpath, STR_POINT) == 0) {
    return CREATE_OK;
  }

  if (dirpath_len == strlen(TWO_POINTS) && strcmp(dirpath, TWO_POINTS) == 0) {
    return CREATE_OK;
  }

  size_t start_index = 2;

  for (size_t i = start_index; (i + 1) < dirpath_len; ++i) {
    if (dirpath[i] == dirpath[i + 1] && dirpath[i] == POINT) {
      return CREATE_INVALID_DIRPATH;
    }
  }

  start_index = 0;

  for (size_t i = start_index; (i + 1) < dirpath_len; ++i) {
    if (dirpath[i] == dirpath[i + 1] && dirpath[i] == SEP_CHAR) {
      return CREATE_INVALID_DIRPATH;
    }
  }

  for (size_t i = 0; i < dirpath_len; ++i) {
    if (!isalnum((unsigned char)dirpath[i]) && dirpath[i] != SEP_CHAR &&
        dirpath[i] != HYPHEN && dirpath[i] != UNDERSCORE &&
        dirpath[i] != POINT) {
      return CREATE_INVALID_DIRPATH;
    }
  }

  return CREATE_OK;
}

CreateState create_file(const char *filepath, size_t fpath_len) {
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
    copy_filepath = NULL;
    return res;
  }

  if (filename == NULL) {
    free(copy_filepath);
    copy_filepath = NULL;
    return CREATE_INVALID_FILENAME;
  }

  res = validate_filename(filename, strlen(filename));

  if (res != CREATE_OK) {
    free(copy_filepath);
    copy_filepath = NULL;
    return res;
  }

  char *dirpath = NULL;

  res = extract_directory_path(&dirpath, copy_filepath, fpath_len);
  if (res != CREATE_OK && res != CREATE_NO_DIRPATH) {
    free(copy_filepath);
    copy_filepath = NULL;
    return res;
  }

  if (dirpath == NULL) {
    free(copy_filepath);
    copy_filepath = NULL;
    return CREATE_INVALID_DIRPATH;
  }

  res = validate_dirpath(dirpath, strlen(dirpath));

  if (res != CREATE_OK) {
    free(copy_filepath);
    copy_filepath = NULL;
    return res;
  }

  if (!is_dir_exist(dirpath)) {
    free(copy_filepath);
    copy_filepath = NULL;
    return CREATE_DIRPATH_DONT_EXIST;
  }

  mode_t mode = 0644;

  int fd = open(filepath, O_CREAT | O_EXCL | O_WRONLY, mode);
  if (fd < 0 && errno == EEXIST) {
    free(copy_filepath);
    copy_filepath = NULL;
    return CREATE_FILE_ALREADY_EXISTS;
  } else if (fd < 0 && errno != EEXIST) {
    free(copy_filepath);
    copy_filepath = NULL;
    return CREATE_UNKNOWN_ERROR;
  }

  close(fd);

  free(copy_filepath);
  copy_filepath = NULL;

  return CREATE_OK;
}

CreateState choose_option(char *option, char **argv, int argc) {
  AvaibleOption avaible_options[] = {{"--df", DIFFERENT_FILEPATH},
                                     {"--sd", SAME_DIR},
                                     {"--dd", DIFFERENT_DIRS}};

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
    return create_different_filepath(argv, argc);
  case SAME_DIR:
    return create_in_same_dir(argv[0], argv + 1, argc - 1);
  case DIFFERENT_DIRS:
    return create_different_dir(argv, argc);
  default:
    return CREATE_INVALID_OPTION;
  }
}

CreateState create_different_dir(char **argv, int argc) {
  CreateState state = CREATE_OK;
  char **copy_argv = (char **)malloc(sizeof(char *) * argc);
  if (copy_argv == NULL) {
    return CREATE_UNKNOWN_ERROR;
  }

  for (int i = 0; i < argc; ++i) {
    size_t arg_len = strlen(argv[i]);
    copy_argv[i] = (char *)malloc(sizeof(char) * (arg_len + 1));
    if (copy_argv[i] == NULL) {
      for (int j = 0; j < i; ++j) {
        free(copy_argv[j]);
        copy_argv[j] = NULL;
      }
      free(copy_argv);
      copy_argv = NULL;
      return CREATE_UNKNOWN_ERROR;
    }
    snprintf(copy_argv[i], (arg_len + 1), "%s", argv[i]);
    int list_len = 0;
    if (count_args(&list_len, copy_argv[i], arg_len) == -1) {
      for (int j = 0; j <= i; ++j) {
        free(copy_argv[j]);
        copy_argv[j] = NULL;
      }
      free(copy_argv);
      copy_argv = NULL;
      return CREATE_UNKNOWN_ERROR;
    }

    char **list = (char **)malloc(sizeof(char *) * list_len);
    if (list == NULL) {
      for (int j = 0; j <= i; ++j) {
        free(copy_argv[j]);
        copy_argv[j] = NULL;
      }
      free(copy_argv);
      copy_argv = NULL;
      return CREATE_UNKNOWN_ERROR;
    }

    if (!extract_list(list_len, &list, copy_argv[i], arg_len)) {
      free(list);
      list = NULL;
      for (int j = 0; j <= i; ++j) {
        free(copy_argv[j]);
        copy_argv[j] = NULL;
      }
      free(copy_argv);
      copy_argv = NULL;
      return CREATE_INVALID_ARGUMENT;
    }

    char **plist = (list + 1);

    state = create_in_same_dir(list[0], plist, (list_len - 1));
    if (state != CREATE_OK) {
      free(list);
      list = NULL;
      for (int j = 0; j <= i; ++j) {
        free(copy_argv[j]);
        copy_argv[j] = NULL;
      }
      free(copy_argv);
      copy_argv = NULL;
      return state;
    }

    free(list);
    list = NULL;
  }
  for (int j = 0; j < argc; ++j) {
    free(copy_argv[j]);
    copy_argv[j] = NULL;
  }
  free(copy_argv);
  copy_argv = NULL;

  return state;
}

CreateState create_different_filepath(char **argv, int argc) {
  CreateState state = CREATE_OK;
  for (int i = 0; i < argc; ++i) {
    state = create_file(argv[i], strlen(argv[i]));
    if (state != CREATE_OK) {
      return state;
    }
  }

  return state;
}

CreateState create_in_same_dir(char *dirpath, char **argv, int argc) {

  if (!is_dir_exist(dirpath)) {
    return CREATE_INVALID_DIRPATH;
  }

  const int fail_to_print = -1;
  CreateState state = CREATE_OK;

  for (int i = 0; i < argc; ++i) {
    size_t fpath_len = strlen(dirpath) + strlen(argv[i]);
    char *filepath = (char *)malloc(sizeof(dirpath[0]) * (fpath_len + 1));
    if (filepath == NULL) {
      return CREATE_UNKNOWN_ERROR;
    }

    if (snprintf(filepath, (fpath_len + 1), "%s%s", dirpath, argv[i]) ==
        fail_to_print) {
      free(filepath);
      filepath = NULL;
      return CREATE_UNKNOWN_ERROR;
    }

    state = create_file(filepath, fpath_len);

    if (state != CREATE_OK) {
      free(filepath);
      filepath = NULL;
      return state;
    }
    free(filepath);
    filepath = NULL;
  }

  return state;
}

static bool is_dir_exist(const char *dirpath) {
  struct stat st;
  if (stat(dirpath, &st) == 0) {
    return S_ISDIR(st.st_mode);
  }
  return false;
}

static int count_args(int *count, char *argument, size_t argument_len) {
  int res = -1;

  if (count == NULL || argument == NULL || argument_len == 0) {
    return res;
  }

  for (size_t i = 0; i < argument_len; ++i) {
    if (argument[i] == FILE_DELIMITATOR) {
      ++(*count);
    }
  }

  ++(*count);
  res = 0;

  return res;
}

static bool is_arg_valid(const char *argument, size_t argument_len) {
  size_t i = argument_len - 1;
  while ((i - 1) > 0 && argument[i - 1] != SEP_CHAR) {
    --i;
  }

  if (argument[i] == argument[i - 1] && argument[i] == SEP_CHAR) {
    return false;
  } else if (argument[i] == FILE_DELIMITATOR && argument[i - 1] != SEP_CHAR) {
    return false;
  }
  return true;
}

static bool extract_list(int list_len, char ***list, char *argument,
                         size_t argument_len) {
  if (list == NULL || *list == NULL || list_len <= 0 || argument_len <= 0 ||
      !is_arg_valid(argument, argument_len)) {
    return false;
  }

  int count = 0;
  *((*list) + count) = argument;
  ++count;

  for (size_t i = 0; i < argument_len && count < list_len; ++i) {
    if (argument[i] == FILE_DELIMITATOR && (i + 1) < argument_len) {
      *(*(list) + count) = argument + (i + 1);
      argument[i] = END_STR;
      ++count;
    }
  }

  return true;
}
