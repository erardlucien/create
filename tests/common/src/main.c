// standart library
#include <stddef.h>
#include <stdio.h>
#include <string.h>
// header project
#include "../../../common/include/files.h"
#include "../include/my_assert.h"

int main(void) {
  char *filenames[] = {NULL,        "main.c",       "main.cpp",
                       "solid.hpp", "log-file.txt", "demo arrest.xml"};
  CreateState states[] = {CREATE_NULL_POINTER_EXCEPTION, CREATE_OK,
                          CREATE_INVALID_FILENAME};
  bool res = ASSERT_FUNC_EQ1(
      states[0], REGISTER_ENTRY1(validate_filename, filenames[0], 1));
  if (!res) {
    fprintf(stderr, "\n");
  }

  for (size_t i = 1; i < sizeof(filenames) / sizeof(filenames[0]); ++i) {
    res = ASSERT_FUNC_EQ1(
        states[1],
        REGISTER_ENTRY1(validate_filename, filenames[i], strlen(filenames[i])));
    if (!res) {
      fprintf(stderr, "\n");
    }
  }

  res = ASSERT_FUNC_NEQ1(states[1],
                         REGISTER_ENTRY1(validate_filename, filenames[0], 0));
  if (!res) {
    fprintf(stderr, "\n");
  }

  for (size_t i = 1; i < sizeof(filenames) / sizeof(filenames[0]); ++i) {
    res = ASSERT_FUNC_NEQ1(
        states[0],
        REGISTER_ENTRY1(validate_filename, filenames[i], strlen(filenames[i])));
    if (!res) {
      fprintf(stderr, "\n");
    }
  }
  char *filetypes[] = {".c",    ".",     ".hpp", ".cpp", ".xml",
                       ".json", ".html", " ",    ""};

  for (size_t i = 1; i < sizeof(filetypes) / sizeof(filetypes[0]); ++i) {
    res = ASSERT_FUNC_NEQ1(
        states[1],
        REGISTER_ENTRY1(validate_filename, filetypes[i], strlen(filetypes[i])));
    if (!res) {
      fprintf(stderr, "\n");
    }
  }

  res = ASSERT_FUNC_NEQ1(states[1],
                         REGISTER_ENTRY1(validate_filename, "main.c", 2));
  if (!res) {
    fprintf(stderr, "\n");
  }

  char dummy[] = "dummy";

  res = ASSERT_FUNC_EQ1(
      states[2], REGISTER_ENTRY1(validate_filename, dummy, strlen(dummy)));
  if (!res) {
    fprintf(stderr, "\n");
  }

  res =
      ASSERT_FUNC_EQ1(states[2], REGISTER_ENTRY1(validate_filename, dummy, 10));
  if (!res) {
    fprintf(stderr, "\n");
  }
}
