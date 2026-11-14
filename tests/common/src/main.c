// standart library
#include <stddef.h>
#include <stdio.h>
#include <string.h>
// header project
#include "../../../common/include/files.h"
#include "../include/my_assert.h"

static void count_err_failure(int *const perr_count, int *const pfailure_count,
                              myAssertState res);

static int test_count = 0;

int main(void) {
  char *filenames[] = {NULL,        "main.c",       "main.cpp",
                       "solid.hpp", "log-file.txt", "demo arrest.xml"};
  CreateState states[] = {CREATE_NULL_POINTER_EXCEPTION, CREATE_OK,
                          CREATE_INVALID_FILENAME};
  int err_count = 0;
  int failure_count = 0;
  myAssertState res = ASSERT_FUNC_EQ1(
      states[0], REGISTER_ENTRY1(validate_filename, filenames[0], 1));
  count_err_failure(&err_count, &failure_count, res);

  for (size_t i = 1; i < sizeof(filenames) / sizeof(filenames[0]); ++i) {
    res = ASSERT_FUNC_EQ1(
        states[1],
        REGISTER_ENTRY1(validate_filename, filenames[i], strlen(filenames[i])));

    count_err_failure(&err_count, &failure_count, res);
  }

  res = ASSERT_FUNC_NEQ1(states[1],
                         REGISTER_ENTRY1(validate_filename, filenames[0], 0));

  count_err_failure(&err_count, &failure_count, res);

  for (size_t i = 1; i < sizeof(filenames) / sizeof(filenames[0]); ++i) {
    res = ASSERT_FUNC_NEQ1(
        states[0],
        REGISTER_ENTRY1(validate_filename, filenames[i], strlen(filenames[i])));
    count_err_failure(&err_count, &failure_count, res);
  }
  char *filetypes[] = {".c",    ".",     ".hpp", ".cpp", ".xml",
                       ".json", ".html", " ",    ""};

  for (size_t i = 1; i < sizeof(filetypes) / sizeof(filetypes[0]); ++i) {
    res = ASSERT_FUNC_NEQ1(
        states[1],
        REGISTER_ENTRY1(validate_filename, filetypes[i], strlen(filetypes[i])));
    count_err_failure(&err_count, &failure_count, res);
  }

  res = ASSERT_FUNC_NEQ1(states[1],
                         REGISTER_ENTRY1(validate_filename, "main.c", 2));
  count_err_failure(&err_count, &failure_count, res);

  char dummy[] = "dummy";

  res = ASSERT_FUNC_EQ1(
      states[2], REGISTER_ENTRY1(validate_filename, dummy, strlen(dummy)));
  count_err_failure(&err_count, &failure_count, res);

  res =
      ASSERT_FUNC_EQ1(states[2], REGISTER_ENTRY1(validate_filename, dummy, 10));
  count_err_failure(&err_count, &failure_count, res);

  res = ASSERT_FUNC_EQ1(states[2], REGISTER_ENTRY1(NULL, dummy, 10));
  count_err_failure(&err_count, &failure_count, res);

  if (err_count > 0 || failure_count > 0) {
    printf("Errors: %d, Failure: %d, Success: %d\n", err_count, failure_count,
           (test_count - (err_count + failure_count)));
    return -1;
  } else {
    printf("All Tests are succeed!\n");
  }

  return 0;
}

static void count_err_failure(int *const perr_count, int *const pfailure_count,
                              myAssertState res) {
  ++test_count;
  if (res == MY_ASSERT_ERROR) {
    ++(*perr_count);
    return;
  }
  if (res == MY_ASSERT_FAILURE) {
    ++(*pfailure_count);
  }
}
