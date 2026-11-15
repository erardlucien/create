// standart library
#include <stddef.h>
#include <stdio.h>
#include <string.h>
// header project
#include "../../../common/include/files.h"
#include "../include/my_assert.h"

struct AssertFilename {
  const char *str;
  CreateState state;
};

struct AssertCounter {
  int err_count;
  int failure_count;
  int test_count;
};

typedef struct AssertFilename AssertFilename;
typedef struct AssertCounter AssertCounter;
static void count_err_failure(AssertCounter *const counter, myAssertState res);

int main(void) {
  AssertCounter counter = {0, 0, 0};
  const AssertFilename assert_filenames[] = {
      {NULL, CREATE_NULL_POINTER_EXCEPTION},
      {"main.cpp", CREATE_OK},
      {".", CREATE_INVALID_FILENAME},
      {".xml", CREATE_INVALID_FILENAME},
      {"", CREATE_INVALID_FILENAME},
      {".m.c", CREATE_INVALID_FILENAME},
      {"tom-mio-n.txt", CREATE_OK},
      {"Angel Zu.html", CREATE_OK},
      {"u\\uio", CREATE_INVALID_FILENAME},
      {"4.c", CREATE_OK},
      {"empty", CREATE_INVALID_FILENAME},
      {"a. b.c", CREATE_INVALID_FILENAME},
      {"a.", CREATE_INVALID_FILENAME}};
  myAssertState res = ASSERT_FUNC_EQ1(CREATE_NULL_POINTER_EXCEPTION,
                                      REGISTER_ENTRY1(NULL, NULL, 1));
  count_err_failure(&counter, res);

  const size_t assert_filenames_len =
      sizeof(assert_filenames) / sizeof(assert_filenames[0]);

  for (size_t i = 0; i < assert_filenames_len; ++i) {
    res = ASSERT_FUNC_EQ1(
        assert_filenames[i].state,
        REGISTER_ENTRY1(validate_filename, assert_filenames[i].str,
                        assert_filenames[i].str == NULL
                            ? 0
                            : strlen(assert_filenames[i].str)));
    count_err_failure(&counter, res);
  }

  if (counter.err_count > 0 || counter.failure_count > 0) {
    printf("Errors: %d, Failure: %d, Success: %d\n", counter.err_count,
           counter.failure_count,
           (counter.test_count - (counter.err_count + counter.failure_count)));
    return -1;
  } else {
    printf("All Tests are passed!\n");
  }

  return 0;
}

static void count_err_failure(AssertCounter *const counter, myAssertState res) {
  ++counter->test_count;
  switch (res) {
  case MY_ASSERT_ERROR:
    ++counter->err_count;
    break;
  case MY_ASSERT_FAILURE:
    ++counter->err_count;
    break;
  default:
    break;
  }
}
