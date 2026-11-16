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

#define SEP_STR "/"
#define SEP_CHAR '/'
#define TWO_POINTS ".."
#define POINT '.'
#define STR_POINT "."
#define HYPHEN '-'
#define UNDERSCORE '_'

static bool is_dir_exist(const char *dirpath);

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

  mode_t mode = 0755;

  int fd = open(filepath, O_RDONLY | O_CREAT, mode);
  close(fd);
  if (fd < 1 && errno == EEXIST) {
    free(copy_filepath);
    copy_filepath = NULL;
    return CREATE_FILE_ALREADY_EXISTS;
  } else if (fd < 1 && errno != EEXIST) {
    free(copy_filepath);
    copy_filepath = NULL;
    return CREATE_UNKNOWN_ERROR;
  }

  free(copy_filepath);
  copy_filepath = NULL;

  return CREATE_OK;
}

bool is_dir_exist(const char *dirpath) {
  struct stat st;
  if (stat(dirpath, &st) == 0) {
    return S_ISDIR(st.st_mode);
  }
  return false;
}
