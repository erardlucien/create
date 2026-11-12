#ifndef _TEST_VALIDATION_H_LER_
#define _TEST_VALIDATION_H_LER_
// standart-libary

// custom-libary
#include "../../../common/include/files.h"

// Expect the dirname to be a valid name
CreateState test_validate_dirname_ok(const char *valid_dirname,
                                     size_t dirname_len);
// Expect the filename to be a valid name
CreateState test_validate_filename_ok(const char *valid_filename,
                                      size_t filename_len);
// Expect the dirname to be invalid
CreateState test_validate_dirname_nok(const char *invalid_dirname,
                                      size_t dirname_len);
// expect the filename to be invalid
CreateState test_validate_filename_nok(const char *invalid_filename,
                                       size_t filename_len);

// Expect the dirname to be NULL
CreateState test_validate_dirname_null();
// expect the filename to be NULL
CreateState test_validate_filename_null();
#endif
