// standart library
#include <stdio.h>
#include <string.h>
// project's header
#include "../include/my_assert.h"

#define LIST_SEP ','
#define NULL_AS_STR "NULL"

static AssertResult list_to_str(char **list_to_str, char **list, int list_len);

AssertResult assert_eq_impl(FunctionEntry entry, CreateState expected,
                            int line) {

  if (entry.entry == NULL) {
    fprintf(stderr, "[ERROR] Null-pointer, entry.entry := NULL at line %d\n",
            line);
    return ASSERT_NULL_POINTER_ERROR;
  }
  CreateState actual = CREATE_OK;
  switch (entry.type) {
  case FUNCTION_TYPE_1: {
    FunctionEntry1 *entry_copy = (FunctionEntry1 *)entry.entry;
    if (entry_copy->fptr == NULL) {
      fprintf(stderr, "[ERROR] Null-pointer, entry.fptr := NULL at line %d\n",
              line);
      return ASSERT_NULL_POINTER_ERROR;
    }
    actual = entry_copy->fptr(entry_copy->str, entry_copy->str_len);
    if (actual != expected) {
      fprintf(stderr,
              "[FAILURE] Failure at line %d, function %s for call %s(%s, %zu), "
              "actual: %d, expected: %d\n",
              line, __func__, entry_copy->fptr_name,
              entry_copy->str == NULL ? "NULL" : entry_copy->str,
              entry_copy->str_len, actual, expected);
      return ASSERT_FAILURE;
    }
  } break;
  case FUNCTION_TYPE_2: {
    FunctionEntry2 *entry_copy = (FunctionEntry2 *)entry.entry;
    if (entry_copy->fptr == NULL) {
      fprintf(stderr, "[ERROR] Null-pointer, entry.fptr := NULL at line %d\n",
              line);
      return ASSERT_NULL_POINTER_ERROR;
    }
    actual = entry_copy->fptr(entry_copy->list, entry_copy->str,
                              entry_copy->str_len);
    if (actual != expected) {
      fprintf(
          stderr,
          "[FAILURE] Failure at line %d, function %s for call %s(%s, %s, %zu), "
          "actual: %d, expected: %d\n",
          line, __func__, entry_copy->fptr_name, "NULL",
          entry_copy->str == NULL ? "NULL" : entry_copy->str,
          entry_copy->str_len, actual, expected);
      return ASSERT_FAILURE;
    }
  } break;
  case FUNCTION_TYPE_3: {
    FunctionEntry3 *entry_copy = (FunctionEntry3 *)entry.entry;
    if (entry_copy->fptr == NULL) {
      fprintf(stderr, "[ERROR] Null-pointer, entry.fptr := NULL at line %d\n",
              line);
      return ASSERT_NULL_POINTER_ERROR;
    }
    actual = entry_copy->fptr(entry_copy->str, entry_copy->list,
                              entry_copy->list_len);
    if (actual != expected) {
      char *arg_str = NULL;
      AssertResult check =
          list_to_str(&arg_str, entry_copy->list, entry_copy->list_len);
      if (check == ASSERT_UNKNOWN_ERROR) {
        fprintf(stderr, "Unknown error\n");
        return check;
      }
      fprintf(stderr,
              "[FAILURE] Failure at line %d, function %s for call %s(%s, [%s], "
              "%d), "
              "actual: %d, expected: %d\n",
              line, __func__, entry_copy->fptr_name,
              entry_copy->str == NULL ? "NULL" : entry_copy->str, arg_str,
              entry_copy->list_len, actual, expected);
      free(arg_str);
      arg_str = NULL;
      return ASSERT_FAILURE;
    }
  } break;
  case FUNCTION_TYPE_4: {
    FunctionEntry4 *entry_copy = (FunctionEntry4 *)entry.entry;
    if (entry_copy->fptr == NULL) {
      fprintf(stderr, "[ERROR] Null-pointer, entry.fptr := NULL at line %d\n",
              line);
      return ASSERT_NULL_POINTER_ERROR;
    }
    actual = entry_copy->fptr(entry_copy->list, entry_copy->list_len);
    if (actual != expected) {
      char *arg_str = NULL;
      AssertResult check =
          list_to_str(&arg_str, entry_copy->list, entry_copy->list_len);
      if (check == ASSERT_UNKNOWN_ERROR) {
        fprintf(stderr, "Unknown error\n");
        return check;
      }
      fprintf(
          stderr,
          "[FAILURE] Failure at line %d, function %s for call %s([%s], %d), "
          "actual: %d, expected: %d\n",
          line, __func__, entry_copy->fptr_name, arg_str, entry_copy->list_len,
          actual, expected);
      free(arg_str);
      arg_str = NULL;
      return ASSERT_FAILURE;
    }
  } break;
  default:
    fprintf(stderr, "Unknown error\n");
    return ASSERT_UNKNOWN_ERROR;
  }
  return ASSERT_SUCCESS;
}

void counting(AssertCounter *pcounter, AssertResult res) {
  switch (res) {
  case ASSERT_NULL_POINTER_ERROR:
    ++pcounter->err_counter;
    break;
  case ASSERT_ERROR:
    ++pcounter->err_counter;
    break;
  case ASSERT_SUCCESS:
    ++pcounter->success_counter;
    break;
  case ASSERT_FAILURE:
    ++pcounter->fail_counter;
    break;
  case ASSERT_UNKNOWN_ERROR:
    ++pcounter->err_counter;
    break;
  default:
    break;
  }
}

static AssertResult list_to_str(char **list_to_str, char **list, int list_len) {
  if (list == NULL || *list == NULL) {
    *list_to_str = NULL_AS_STR;
    return ASSERT_SUCCESS;
  }
  size_t len = 0;
  for (int i = 0; i < list_len; ++i) {
    if (list[i] == NULL) {
      len += strlen(NULL_AS_STR);
    } else {
      len += strlen(list[i]);
    }
  }

  ++len;

  *list_to_str = (char *)malloc(len + list_len + 1);
  if (*list_to_str == NULL) {
    return ASSERT_UNKNOWN_ERROR;
  }

  for (int i = 0; i < list_len; ++i) {
    if (list[i] == NULL) {
      strcpy(*list_to_str, "NULL");
    } else {
      strcpy(*list_to_str, list[i]);
      (*list_to_str)[strlen(list[i]) + 1] = LIST_SEP;
    }
  }

  return ASSERT_SUCCESS;
}
