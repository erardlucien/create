// standart library
// header-project
#include "../../common/include/my_assert.h"
#include <stdio.h>

bool my_assert_func0(func0_t fptr, const char *str, size_t len, int line,
                     char *func_name, CreateState expected) {
  if (fptr == NULL || func_name == NULL || str == NULL) {
    fprintf(stderr, "[ERROR] Null-Pointer was given %d", line);
    return false;
  }

  CreateState actual = fptr(str, len);

  if (actual != expected) {
    fprintf(stderr,
            "[FAILED] %s for function %s(%s, %zu) expected: %d, actual: %d at "
            "line %d",
            __func__, func_name, str, len, expected, actual, line);
    return false;
  }

  return true;
}

bool my_assert_func1(func1_t fptr, char *str1, const char *str2,
                     size_t str1_len, size_t str2_len, int line,
                     char *func_name, CreateState expected) {
  if (fptr == NULL || func_name == NULL || str1 == NULL || str2 == NULL) {
    fprintf(stderr, "[ERROR] Null-Pointer was given %d", line);
    return false;
  }

  CreateState actual = fptr(str1, str2, str1_len, str2_len);

  if (actual != expected) {
    fprintf(stderr,
            "[FAILED] %s for function %s(%s, %s, %zu, %zu) expected: %d, "
            "actual: %d at "
            "line %d",
            __func__, func_name, str1, str2, str1_len, str2_len, expected,
            actual, line);
    return false;
  }
  return true;
}

bool my_assert_func2(func2_t fptr, char **pstr, const char *str,
                     size_t pstr_len, size_t str_len, int line, char *func_name,
                     CreateState expected) {
  if (fptr == NULL || func_name == NULL || str == NULL) {
    fprintf(stderr, "[ERROR] Null-Pointer was given %d", line);
    return false;
  }

  CreateState actual = fptr(pstr, str, pstr_len, str_len);

  if (actual != expected) {
    fprintf(stderr,
            "[FAILED] %s for function %s(%s, %s, %zu, %zu) expected: %d, "
            "actual: %d at "
            "line %d",
            __func__, func_name, "pstr", str, pstr_len, str_len, expected,
            actual, line);
    return false;
  }
  return true;
}
