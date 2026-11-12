#ifndef _FILE_H_LER_
#define _FILE_H_LER_
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
  // failed extraction of filename
  CREATE_EXTRACT_FILENANE_FAILED = -8,
  // failed extraction of directory-name
  CREATE_EXTRACT_DIRNAME_FAILED = -9,
  // failed extraction of directory-path
  CREATE_EXTRACT_DIRPATH_FAILED = -10,
  // failed extraction of each parent-directories-names
  CREATE_EXTRACT_DIRNAMES_FAILED = -11,
  // has a separator-character as the last character before '\0'
  CREATE_HASNO_ENDSEP = -12,
  // failed creation dirpath with separator-character at the end
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
CreateState create_file(const char *filepath, size_t fpath_len);
// extract the filename from 'filepath'
CreateState extract_filename(char *filename, const char *filepath,
                             size_t filename_capacity, size_t fpath_len);
// create directory, if it does not exist.
CreateState create_directory(const char *dirpath, size_t dirpath_len);
// extract directory-name from 'dirpath'
CreateState extract_directory_name(char *dirname, const char *dirpath,
                                   size_t dirname_capacity, size_t dirpath_len);
// extract directory-path from 'filepath'
CreateState extract_directory_path(char *dirpath, const char *filepath,
                                   size_t dirpath_capacity,
                                   size_t filepath_len);
// extract each parent-directories-names
CreateState extract_parents_directories(char **dirnames, const char *dirpath,
                                        size_t dirname_capacity,
                                        size_t dirpath_len);
// control if directory-name is valid
CreateState validate_dirname(const char *dirname, size_t dirname_len);
// control if filename is valid
CreateState validate_filename(const char *filename, size_t filename_len);
// check if dirpath has separator-character as the last character before '\0'
CreateState has_endstep(const char *dirpath, size_t dirpath_len);
// cretae a new dirpath with separator-character based on the old dirpath
CreateState generate_directory_path(char *newdir, const char *olddir,
                                    size_t new_len, size_t old_len);
#endif
