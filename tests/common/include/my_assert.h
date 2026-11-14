#ifndef _MY_ASSERT_LER_
#define _MY_ASSERT_LER_
// standart library
#include <stdbool.h>
#include <stddef.h>
// header-project
#include "../../../common/include/files.h"
typedef CreateState (*func0_t)(const char *, size_t);
typedef CreateState (*func1_t)(char *, const char *, size_t, size_t);
typedef CreateState (*func2_t)(char **, const char *, size_t, size_t);

#define ASSERT_FUNC0_EQ(fn, s, len, expected)                                  \
  my_assert_func0(fn, s, len, __LINE__, #fn, (CreateState)expected)

#define ASSERT_FUNC1_EQ(fn, s1, s2, s1_len, s2_len, expected)                  \
  my_assert_func1(fn, s1, s2, s1_len, s2_len, __LINE__, #fn,                   \
                  (CreateState)expected)

#define ASSERT_FUNC2_EQ(fn, ps, s1, ps_len, s1_len, expected)                  \
  my_assert_func2(fn, ps, s1, ps_len, s1_len, __LINE__, #fn,                   \
                  (CreateState)expected)

bool my_assert_func0(func0_t fptr, const char *str, size_t len, int line,
                     char *func_name, CreateState expected);

bool my_assert_func1(func1_t fptr, char *str1, const char *str2,
                     size_t str1_len, size_t str2_len, int line,
                     char *func_name, CreateState expected);

bool my_assert_func2(func2_t fptr, char **pstr, const char *str,
                     size_t pstr_len, size_t str_len, int line, char *func_name,
                     CreateState expected);
#endif
