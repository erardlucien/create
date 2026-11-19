// standart library
#include <stdlib.h>
// project's library
#include "../../../common/include/files.h"
#include "../include/my_assert.h"

int main(void) {
  FunctionEntry entry = {&(REGISTER_ENTRY1(validate_dirpath, NULL, 1)),
                         FUNCTION_TYPE_1};
  AssertResult res = ASSERT_EQ(entry, CREATE_NULL_POINTER_EXCEPTION);
  AssertCounter counter;
  counting(&counter, res);
  return EXIT_SUCCESS;
}
