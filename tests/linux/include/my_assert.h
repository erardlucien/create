#ifndef MY_ASSERT_H
#define MY_ASSERT_H
// standart library
// project's header
#include "function_entry.h"

enum AssertResult {
  ASSERT_SUCCESS = 0,
  ASSERT_FAILURE = -1,
  ASSERT_UNKNOWN_ERROR = -2,
  ASSERT_ERROR = -3,
  ASSERT_NULL_POINTER_ERROR = -4,
};

typedef enum AssertResult AssertResult;

struct AssertCounter {
  int fail_counter;
  int err_counter;
  int success_counter;
};

typedef struct AssertCounter AssertCounter;

#define ASSERT_EQ(entry, expected) assert_eq_impl((entry), (expected), __LINE__)

extern AssertResult assert_eq_impl(FunctionEntry entry, CreateState expected,
                                   int line);

extern void counting(AssertCounter *pcounter, AssertResult res);

#endif // !MY_ASSERT_H
