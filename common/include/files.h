#ifndef _FILE_H_LER_
#define _FILE_H_LER_
#include <cstddef>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// All possible value which could be return
enum CreateState {
  // no failure has occurred
  NO_ERROR = 0,
  // something weng wrong, but the error is no recognise
  UNKNOWN_ERROR = -1,
  // the exist already
  FILE_EXIST_ALR = -2,
  // the directory exist already
  DIR_EXIST_ALR = -3,
  // filename is not valid
  NO_VALID_FNAME = -4,
  // directory-name is not valid
  NO_VALID_DIRNAME = -5,
  // filepath is not valid
  INVALID_FPATH = -6,
  // directory-path is not valid
  INVALID_DIRPATH = -7,
  // extraction of filename fails
  EXTC_FNANE_FAIL = -8,
  // extraction of directory-path fails
  EXTC_DIRNAME = -9,
  // extraction of directory-name fails
  EXTC_DIRPATH = -10,
  // extraction of each parent-directories-names fail
  EXTC_DIRS = -11,
  // has a separator-character as the last character before '\0'
  NO_ENDSEP = -12,
  // Creating dirpath with separator-character at the end fail
  NDIRPATH_FAIL = -13,
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
CreateState create(const char *filepath, const size_t *pfpath_len);
// extract the filename from 'filepath'
CreateState extract(const char *filename, const char *filepath,
                    const size_t *pfpath_len);
// create directory, if it does not exist.
CreateState createdr(const char *dirpath, const size_t *pdirpath_len);
// extract directory-name from 'dirpath'
CreateState extractdr(const char *dirname, const char *dirpath,
                      const size_t *pdirpath_len);
// extract directory-path from 'filepath'
CreateState extractdrf(const char *dirpath, const char *filepath,
                       const size_t *pdirpath_len);
// extract each parent-directories-names
CreateState extractdrs(const char **dirname, const char *dirpath,
                       const size_t *pdirpath_len);
// control if directory-name is valid
CreateState controld(const char *dirname, const size_t *pdirname_len);
// control if filename is valid
CreateState control(const char *filename, const size_t *pfilename_len);
// check if dirpath has separator-character as the last character before '\0'
CreateState has_endstep(const char *dirpath, const size_t *pdirpath_len);
// cretae a new dirpath with separator-character based on the old dirpath
CreateState generate(const char *newdir, const char *olddir,
                     const size_t *pnew_len, const size_t *pold_len);
#endif
