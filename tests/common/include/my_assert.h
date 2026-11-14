#ifndef _MY_ASSERT_LER_
#define _MY_ASSERT_LER_
// standart library
#include <stdbool.h>
#include <stddef.h>
// header-project
#include "../../../common/include/files.h"

#define ASSERT_FUNC_EQ(expression)                                             \
  my_assert_func((expression), #expression, __LINE__)

#define ASSERT_FUNC_NEQ(expression)                                            \
  my_assert_func(!(expression), #expression, __LINE__)

bool my_assert_func(bool condition, const char *expression, int line);
#endif
