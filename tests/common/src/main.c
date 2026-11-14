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
  CreateState states[] = {CREATE_NULL_POINTER_EXCEPTION, CREATE_OK};
  bool res = ASSERT_FUNC_EQ(validate_filename(NULL, 1) == states[0]);
  if (!res) {
    fprintf(stderr, "\n");
  }

  for (size_t i = 1; i < sizeof(filenames) / sizeof(filenames[1]); ++i) {
    res = ASSERT_FUNC_EQ(
        validate_filename(filenames[i], strlen(filenames[i])) == states[1]);
    if (!res) {
      fprintf(stderr, "\n");
    }
  }

  res = ASSERT_FUNC_NEQ(validate_filename(filenames[0], 1) == states[1]);
  if (!res) {
    fprintf(stderr, "\n");
  }

  for (size_t i = 1; i < sizeof(filenames) / sizeof(filenames[1]); ++i) {
    res = ASSERT_FUNC_NEQ(
        validate_filename(filenames[i], strlen(filenames[i])) == states[0]);
    if (!res) {
      fprintf(stderr, "\n");
    }
  }
  char *filetypes[] = {".c",   ".",     ".hpp",  ".cpp",
                       ".xml", ".json", ".html", " "};

  for (size_t i = 1; i < sizeof(filetypes) / sizeof(filetypes[0]); ++i) {
    res = ASSERT_FUNC_NEQ(
        validate_filename(filetypes[i], strlen(filetypes[i])) == states[1]);
    if (!res) {
      fprintf(stderr, "\n");
    }
  }
}
