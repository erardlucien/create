#ifndef FUNCTION_ENTRY_H
#define FUNCTION_ENTRY_H
// standart library
// project's header
#include "../../../common/include/files.h"

typedef void *pfunction_entry;

enum FunctionType {
  FUNCTION_TYPE_1 = 0,
  FUNCTION_TYPE_2 = 1,
  FUNCTION_TYPE_3 = 2,
  FUNCTION_TYPE_4 = 3,
};

typedef enum FunctionType FunctionType;

struct FunctionEntry {
  pfunction_entry entry;
  FunctionType type;
};

typedef struct FunctionEntry FunctionEntry;

typedef CreateState (*fptr1)(const char *, size_t);
typedef CreateState (*fptr2)(char **, char *, size_t);
typedef CreateState (*fptr3)(char *, char **, int);
typedef CreateState (*fptr4)(char **, int);

struct FunctionEntry1 {
  fptr1 fptr;
  const char *fptr_name;
  const char *str;
  size_t str_len;
};

struct FunctionEntry2 {
  fptr2 fptr;
  const char *fptr_name;
  char **list;
  char *str;
  size_t str_len;
};

struct FunctionEntry3 {
  fptr3 fptr;
  const char *fptr_name;
  char *str;
  char **list;
  int list_len;
};

struct FunctionEntry4 {
  fptr4 fptr;
  const char *fptr_name;
  char **list;
  int list_len;
};

typedef struct FunctionEntry1 FunctionEntry1;
typedef struct FunctionEntry2 FunctionEntry2;
typedef struct FunctionEntry3 FunctionEntry3;
typedef struct FunctionEntry4 FunctionEntry4;

#define REGISTER_ENTRY1(fn, str, str_len)                                      \
  (FunctionEntry1) { (fn), (fn) == NULL ? "NULL" : #fn, (str), (str_len) }
#define REGISTER_ENTRY2(fn, list, str, str_len)                                \
  (FunctionEntry2) {                                                           \
    (fn), (fn) == NULL ? "NULL" : #fn, (list), (str), (str_len)                \
  }
#define REGISTER_ENTRY3(fn, str, list, list_len)                               \
  (FunctionEntry3) {                                                           \
    (fn), (fn) == NULL ? "NULL" : #fn, (str), (list), (list_len)               \
  }
#define REGISTER_ENTRY4(fn, list, list_len)                                    \
  (FunctionEntry4) { (fn), (fn) == NULL ? "NULL" : #fn, (list), (list_len) }

#endif // !FUNCTION_ENTRY_H
