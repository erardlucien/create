// standart library
#include <asm-generic/errno-base.h>
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
#include "../../common/include/create.h"

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
