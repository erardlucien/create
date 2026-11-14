// standart library
#include <stdio.h>
// header-project
#include "../../common/include/my_assert.h"

bool my_assert_func(bool condition, const char *expression, int line) {
  if (expression == NULL) {
    fprintf(stderr, "[ERROR] Null-Pointer was given %d", line);
    return false;
  }

  if (!condition) {
    fprintf(stderr, "[FAILED] %s for the expression %s at line %d", __func__,
            expression, line);
    return false;
  }

  return true;
}
