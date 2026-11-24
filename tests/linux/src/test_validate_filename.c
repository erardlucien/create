// standart library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// project's header
#include "../include/my_assert.h"

int main(void) {
  AssertEntry tests[] = {
      {{&REGISTER_ENTRY1(validate_filename, NULL, 1), FUNCTION_TYPE_1},
       CREATE_NULL_POINTER_EXCEPTION},
      {{&REGISTER_ENTRY1(validate_filename, "*a.c", strlen("*a.c")),
        FUNCTION_TYPE_1},
       CREATE_INVALID_FILENAME},
      {{&REGISTER_ENTRY1(validate_filename, "?.txt", strlen("?.txt")),
        FUNCTION_TYPE_1},
       CREATE_INVALID_FILENAME},
      {{&REGISTER_ENTRY1(validate_filename, "*", strlen("*")), FUNCTION_TYPE_1},
       CREATE_INVALID_FILENAME},
      {{&REGISTER_ENTRY1(validate_filename, "ü.txt", strlen("ü.txt")),
        FUNCTION_TYPE_1},
       CREATE_INVALID_FILENAME},
      {{&REGISTER_ENTRY1(validate_filename, " .txt", strlen(" .txt")),
        FUNCTION_TYPE_1},
       CREATE_INVALID_FILENAME},
      {{&REGISTER_ENTRY1(validate_filename, ".m.html", strlen(".m.html")),
        FUNCTION_TYPE_1},
       CREATE_INVALID_FILENAME},
      {{&REGISTER_ENTRY1(validate_filename, ".m.", strlen(".m.")),
        FUNCTION_TYPE_1},
       CREATE_INVALID_FILENAME},
      {{&REGISTER_ENTRY1(validate_filename, "?.txt", strlen("?.txt")),
        FUNCTION_TYPE_1},
       CREATE_INVALID_FILENAME},
      {{&REGISTER_ENTRY1(validate_filename, ".txt", strlen(".txt")),
        FUNCTION_TYPE_1},
       CREATE_INVALID_FILENAME},
  };

  AssertCounter counter = {0, 0, 0};

  for (int i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i) {
    AssertResult res = ASSERT_EQ(tests[i].entry, tests[i].expected);
    counting(&counter, res);
  }

  printf("Success: %d, Error: %d, Failure: %d", counter.success_counter,
         counter.err_counter, counter.fail_counter);

  if (counter.err_counter > 0 || counter.fail_counter > 0) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
