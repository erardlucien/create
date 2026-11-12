// standart-libary
#include <stddef.h>
// custom-libary
#include "../include/test_validation.h"

CreateState test_validate_dirname_nok(const char *invalid_dirname,
                                      size_t dirname_len) {
  if (invalid_dirname == NULL) {
    return CREATE_NULL_POINTER_EXCEPTION;
  }
  return validate_dirname(invalid_dirname, dirname_len);
}

CreateState test_validate_filename_ok(const char *valid_filename,
                                      size_t filename_len) {
  if (valid_filename == NULL) {
    return CREATE_NULL_POINTER_EXCEPTION;
  }
  return validate_filename(valid_filename, filename_len);
}

CreateState test_validate_dirname_ok(const char *valid_dirname,
                                     size_t dirname_len) {
  if (valid_dirname == NULL) {
    return CREATE_NULL_POINTER_EXCEPTION;
  }
  return validate_dirname(valid_dirname, dirname_len);
}

CreateState test_validate_filename_nok(const char *invalid_filename,
                                       size_t filename_len) {
  if (invalid_filename == NULL) {
    return CREATE_NULL_POINTER_EXCEPTION;
  }
  return validate_filename(invalid_filename, filename_len);
}

CreateState test_validate_dirname_null() {
  return validate_dirname(NULL, (size_t)0);
}

CreateState test_validate_filename_null() {
  return validate_filename(NULL, (size_t)0);
}
