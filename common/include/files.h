#ifndef _FILE_H_LER_
#define _FILE_H_LER_
#include <cstddef>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// All possible value which could be return
enum CreateState {
  // no failure has occurred
  CREATE_OK = 0,
  // something weng wrong, but the error is no recognise
  CREATE_UNKNOWN_ERROR = -1,
  // the exist already
  CREATE_FILE_ALREADY_EXISTS = -2,
  // the directory exist already
  CREATE_DIR_ALREADY_EXISTS = -3,
  // filename is not valid
  CREATE_INVALID_FILENAME = -4,
  // directory-name is not valid
  CREATE_INVALID_DIRNAME = -5,
  // filepath is not valid
  CREATE_INVALID_FILEPATH = -6,
  // directory-path is not valid
  CREATE_INVALID_DIRPATH = -7,
  // extraction of filename fails
  CREATE_EXTRACT_FILENANE_FAILED = -8,
  // extraction of directory-name fails
  CREATE_EXTRACT_DIRNAME_FAILED = -9,
  // extraction of directory-path fails
  CREATE_EXTRACT_DIRPATH_FAILED = -10,
  // extraction of each parent-directories-names fail
  CREATE_EXTRACT_DIRNAMES_FAILED = -11,
  // has a separator-character as the last character before '\0'
  CREATE_HASNO_ENDSEP = -12,
  // Creating dirpath with separator-character at the end fail
  CREATE_NEWDIRPATH_FAIL = -13,
};

#define END_STR '\0'
// special character, which are allowed
#define ALLOWED_SCHAR {':', '.', '/', '\\', ' ', '-'}
// separator-character for windows
#define WIN_SEP "\\"
// separator-character for linux
#define LIN_SEP "/"

typedef enum CreateState CreateState;
// Create a file with the help of the 'filepath'
// fpath_len is the length of the string filepath
CreateState create_file(const char *filepath, const size_t fpath_len);
// extract the filename from 'filepath'
CreateState extract_filename(const char *filename, const char *filepath,
                             const size_t fpath_len);
// create directory, if it does not exist.
CreateState create_directory(const char *dirpath, const size_t dirpath_len);
// extract directory-name from 'dirpath'
CreateState extract_directory_name(const char *dirname, const char *dirpath,
                                   const size_t dirname_capacity,
                                   const size_t dirpath_len);
// extract directory-path from 'filepath'
CreateState extract_directory_path(const char *dirpath, const char *filepath,
                                   const size_t dirpath_capacity,
                                   const size_t filepath_len);
// extract each parent-directories-names
CreateState extract_parents_directories(const char **dirnames,
                                        const char *dirpath,
                                        const size_t dirnames_len,
                                        const size_t dirpath_len);
// control if directory-name is valid
CreateState validate_dirname(const char *dirname, const size_t dirname_len);
// control if filename is valid
CreateState validate_filename(const char *filename, const size_t filename_len);
// check if dirpath has separator-character as the last character before '\0'
CreateState has_endstep(const char *dirpath, const size_t dirpath_len);
// cretae a new dirpath with separator-character based on the old dirpath
CreateState generate_directory_path(const char *newdir, const char *olddir,
                                    const size_t new_len, const size_t old_len);
#endif
