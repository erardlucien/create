#ifndef _FILE_H_LER_
#define _FILE_H_LER_
// standart-libary
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
// custom-libary

// All possible value which could be return
enum CreateState {
  // help
  CREATE_HELP = 1,
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
  // failed extraction of directory-path
  CREATE_EXTRACT_DIRPATH_FAILED = -9,
  // Null-pointer exception
  CREATE_NULL_POINTER_EXCEPTION = -10,
  // directory-path does not exist
  CREATE_DIRPATH_DONT_EXIST = -11,
  // unknow option
  CREATE_INVALID_OPTION = -12,
  // invalid argument for creating more files in different dirpaths
  CREATE_INVALID_ARGUMENT = -13,
  // invalid argument counter
  CREATE_INVALID_ARGC = -14,
};

#define END_STR '\0'
#define MAKEFILE "Makefile"

typedef enum CreateState CreateState;
// Create a file with the help of the 'filepath'
// fpath_len is the length of the string filepath
extern CreateState create_file(const char *filepath, size_t fpath_len);
// extract the filename from 'filepath'
extern CreateState extract_filename(char **filename, char *filepath,
                                    size_t fpath_len);
// create directory, if it does not exist.
extern CreateState create_directory(const char *dirpath, size_t dirpath_len);
// extract directory-path from 'filepath'
extern CreateState extract_directory_path(char **dirpath, char *copy_filepath,
                                          size_t filepath_len);
// control if directory-name is valid
extern CreateState validate_dirpath(const char *dirpath, size_t dirpath_len);
// control if filename is valid
extern CreateState validate_filename(const char *filename, size_t filename_len);
#endif
