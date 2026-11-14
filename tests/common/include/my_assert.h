#ifndef MY_ASSERT_H_LER
#define MY_ASSERT_H_LER
// standart library
#include <stdbool.h>
#include <stddef.h>
// header-project
#include "../../../common/include/files.h"

typedef CreateState (*fptr1)(const char *, size_t);

struct FuncEntry1 {
  fptr1 fn;
  const char *fn_name;
  const char *str;
  size_t str_len;
};

typedef struct FuncEntry1 FuncEntry1;

enum myAssertState {
  MY_ASSERT_OK = 1,
  MY_ASSERT_FAILURE = 2,
  MY_ASSERT_ERROR = 3,
};

typedef enum myAssertState myAssertState;

#define REGISTER_ENTRY1(fn, str, str_len)                                      \
  (FuncEntry1) {                                                               \
    (fptr1)(fn), (const char *)((fn) == NULL ? "NULL" : (#fn)),                \
        (const char *)(str), (size_t)(str_len)                                 \
  }

#define ASSERT_FUNC_EQ1(expected, entry)                                       \
  my_assert_eq1((expected), (entry), __LINE__)

#define ASSERT_FUNC_NEQ1(expected, entry)                                      \
  my_assert_neq1((expected), (entry), __LINE__)

myAssertState my_assert_eq1(CreateState expected, FuncEntry1 entry, int line);
myAssertState my_assert_neq1(CreateState expected, FuncEntry1 entry, int line);
#endif
