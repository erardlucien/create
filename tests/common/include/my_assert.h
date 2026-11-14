#ifndef MY_ASSERT_H_LER
#define MY_ASSERT_H_LER
// standart library
#include <stdbool.h>
#include <stddef.h>
// header-project
#include "../../../common/include/files.h"

#define ASSERT_FUNC_EQ(expected, actual)                                       \
  my_assert_eq((expected), (actual), #actual, __LINE__)

#define ASSERT_FUNC_NEQ(expected, actual)                                      \
  my_assert_neq((expected), (actual), #actual, __LINE__)

bool my_assert_eq(CreateState expected, CreateState actual,
                  const char *expression, int line);
bool my_assert_neq(CreateState expected, CreateState actual,
                   const char *expression, int line);
#endif
