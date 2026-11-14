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

struct AssertFilename {
  const char *str;
  CreateState state;
};

typedef struct AssertFilename AssertFilename;

int main(void) {
  int err_count = 0;
  int failure_count = 0;
  const AssertFilename assert_filenames[] = {
      {NULL, CREATE_NULL_POINTER_EXCEPTION}, {"main.cpp", CREATE_OK},
      {".", CREATE_INVALID_FILENAME},        {".xml", CREATE_INVALID_FILENAME},
      {"", CREATE_INVALID_FILENAME},         {".m.c", CREATE_OK},
      {"tom-mio-n.txt", CREATE_OK},          {"Angel Zu.html", CREATE_OK},
      {"u\\uio", CREATE_INVALID_FILENAME},   {"4.c", CREATE_OK},
      {"empty", CREATE_INVALID_FILENAME},
  };
  myAssertState res =
      ASSERT_FUNC_EQ1(CREATE_OK, REGISTER_ENTRY1(NULL, NULL, 1));
  count_err_failure(&err_count, &failure_count, res);

  const size_t assert_filenames_len =
      sizeof(assert_filenames) / sizeof(assert_filenames[0]);

  for (size_t i = 0; i < assert_filenames_len; ++i) {
    res = ASSERT_FUNC_EQ1(
        assert_filenames[i].state,
        REGISTER_ENTRY1(validate_filename, assert_filenames[i].str,
                        assert_filenames[i].str == NULL
                            ? 0
                            : strlen(assert_filenames[i].str)));
    count_err_failure(&err_count, &failure_count, res);
  }

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
