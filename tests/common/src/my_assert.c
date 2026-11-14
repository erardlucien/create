// standart library
#include <stdio.h>
// header-project
#include "../../common/include/my_assert.h"

bool my_assert_eq(CreateState expected, CreateState actual,
                  const char *expression, int line) {
  if (expression == NULL) {
    fprintf(stderr, "[ERROR] Null-expression at line %d", line);
    return false;
  }

  bool condition = expected == actual;

  if (!condition) {
    fprintf(stderr,
            "[FAILED] %s for the expression %s at line %d, expected: %d is not "
            "equal to "
            "actual: %d",
            __func__, expression, line, expected, actual);
  }

  return condition;
}

bool my_assert_neq(CreateState expected, CreateState actual,
                   const char *expression, int line) {
  if (expression == NULL) {
    fprintf(stderr, "[ERROR] Null-expression at line %d", line);
    return false;
  }

  bool condition = expected != actual;

  if (!condition) {
    fprintf(stderr,
            "[FAILED] %s for the expression %s at line %d, expected: %d is "
            "equal to "
            "actual: %d",
            __func__, expression, line, expected, actual);
  }

  return condition;
}
