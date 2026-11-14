// standart-libary
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
// custom-libary
#include "../include/files.h"

static const char *ALLOWED_TYPE_FILE[] = {
    ".c",  ".html", ".cpp", ".h",   ".rs",  ".txt", ".md",   ".css",
    ".py", ".js",   ".hpp", ".csv", ".tex", ".xml", ".json", ".mk"};
static const size_t ALLOWED_TYPE_FILE_LEN =
    sizeof(ALLOWED_TYPE_FILE) / sizeof(ALLOWED_TYPE_FILE[0]);

static const char ALLOWED_CHARACTERS[] = {
    ' ',
    '.',
    '-',
    '_',
};
static size_t ALLOWED_CHARACTER_LEN =
    sizeof(ALLOWED_CHARACTERS) / sizeof(ALLOWED_CHARACTERS[0]);

static bool is_character_valid(char c);
static bool is_valid_type(const char *filename, size_t filename_len);

CreateState validate_filename(const char *filename, size_t filename_len) {
  if (filename == NULL) {
    return CREATE_NULL_POINTER_EXCEPTION;
  }

  if (strlen(filename) != filename_len) {
    return CREATE_INVALID_FILENAME;
  }

  for (size_t i = 0; i < filename_len; ++i) {
    if (!is_character_valid(filename[i])) {
      return CREATE_INVALID_FILENAME;
    }
  }

  return is_valid_type(filename, filename_len) ? CREATE_OK
                                               : CREATE_INVALID_FILENAME;
}

static bool is_character_valid(char c) {
  if (isalnum(c)) {
    return true;
  }

  for (size_t i = 0; i < ALLOWED_CHARACTER_LEN; ++i) {
    if (c == ALLOWED_CHARACTERS[i]) {
      return true;
    }
  }

  return false;
}

static bool is_valid_type(const char *filename, size_t filename_len) {
  if (filename == NULL || strlen(filename) != filename_len) {
    return false;
  }

  if (strcmp(filename, MAKEFILE) == 0) {
    return true;
  }

  for (size_t i = 0; i < ALLOWED_TYPE_FILE_LEN; ++i) {
    size_t len = strlen(ALLOWED_TYPE_FILE[i]);
    const char *top = filename + (strlen(filename) - len);
    if (strncmp(top, ALLOWED_TYPE_FILE[i], len) == 0) {
      return true;
    }
  }
  return false;
}
