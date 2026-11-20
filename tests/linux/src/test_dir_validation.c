// standart library
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// project's library
#include "../../../common/include/files.h"
#include "../include/my_assert.h"

int main(void) {
  AssertCounter counter;
  AssertEntry assert_entries[] = {
      {{&(REGISTER_ENTRY1(validate_dirpath, NULL, 1)), FUNCTION_TYPE_1},
       CREATE_NULL_POINTER_EXCEPTION},
      {{&(REGISTER_ENTRY1(validate_dirpath, "m", 1)), FUNCTION_TYPE_1},
       CREATE_HASNO_ENDSEP},
      {{&(REGISTER_ENTRY1(validate_dirpath, "/home/user-p/docs",
                          strlen("/home/user-p/docs"))),
        FUNCTION_TYPE_1},
       CREATE_HASNO_ENDSEP},
      {{&(REGISTER_ENTRY1(validate_dirpath, ":home/user-p/docs",
                          strlen(":home/user-p/docs"))),
        FUNCTION_TYPE_1},
       CREATE_HASNO_ENDSEP},
      {{&(REGISTER_ENTRY1(validate_dirpath, "/home/user-p/docs/",
                          strlen("/home/user-p/docs/"))),
        FUNCTION_TYPE_1},
       CREATE_OK},
      {{&(REGISTER_ENTRY1(validate_dirpath, "/m+/", strlen("/m+/"))),
        FUNCTION_TYPE_1},
       CREATE_INVALID_DIRPATH},
      {{&(REGISTER_ENTRY1(validate_dirpath, "m/", strlen("/m"))),
        FUNCTION_TYPE_1},
       CREATE_OK},
      {{&(REGISTER_ENTRY1(validate_dirpath, ".m/", strlen(".m/"))),
        FUNCTION_TYPE_1},
       CREATE_INVALID_DIRPATH},
      {{&(REGISTER_ENTRY1(validate_dirpath, ".m/", 1)), FUNCTION_TYPE_1},
       CREATE_INVALID_DIRPATH},
      {{&(REGISTER_ENTRY1(validate_dirpath, "~/docs/", strlen("~/docs/"))),
        FUNCTION_TYPE_1},
       CREATE_OK},
      {{&(REGISTER_ENTRY1(validate_dirpath, "~/./", strlen("~/."))),
        FUNCTION_TYPE_1},
       CREATE_INVALID_DIRPATH},
  };

  for (size_t i = 0; i < sizeof(assert_entries) / sizeof(assert_entries[0]);
       ++i) {
    AssertResult res =
        ASSERT_EQ(assert_entries[i].entry, assert_entries[i].expected);
    counting(&counter, res);
  }
  printf("Success: %d, Error: %d, Failure %d\n", counter.success_counter,
         counter.err_counter, counter.fail_counter);
  if (counter.err_counter > 0 || counter.fail_counter > 0) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
