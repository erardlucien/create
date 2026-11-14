// standart library
#include <stdio.h>
// header-project
#include "../../common/include/my_assert.h"

bool my_assert_eq1(CreateState expected, FuncEntry1 entry, int line) {
  if (entry.fn == NULL) {
    fprintf(stderr, "[ERROR] Null-pointer at line %d", line);
    return false;
  }

  CreateState actual = entry.fn(entry.str, entry.str_len);
  bool condition = expected == actual;

  if (!condition) {
    fprintf(stderr,
            "[FAILED] %s for  actual = %s(%s, %zu) at line %d, expected: %d, "
            "actual: %d",
            __func__, entry.fn_name, entry.str, entry.str_len, line, expected,
            actual);
  }

  return condition;
}

bool my_assert_neq1(CreateState expected, FuncEntry1 entry, int line) {
  if (entry.fn == NULL) {
    fprintf(stderr, "[ERROR] Null-pointer at line %d", line);
    return false;
  }

  CreateState actual = entry.fn(entry.str, entry.str_len);
  bool condition = expected != actual;

  if (!condition) {
    fprintf(stderr,
            "[FAILED] %s for  actual = %s(%s, %zu) at line %d, expected: %d == "
            "actual: %d",
            __func__, entry.fn_name, entry.str, entry.str_len, line, expected,
            actual);
  }

  return condition;
}
