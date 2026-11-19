#ifndef CREATE_H
#define CREATE_H
// standart library
// project's header
#include "../../../common/include/files.h"

#define SEP_STR "/"
#define SEP_CHAR '/'
#define TWO_POINTS ".."
#define POINT '.'
#define STR_POINT "."
#define HYPHEN '-'
#define UNDERSCORE '_'
#define FILE_DELIMITATOR ':'

enum OptionIndex {
  INVALID_OPTION = -1,
  DIFFERENT_FILEPATH = 0,
  SAME_DIR = 1,
  DIFFERENT_DIRS = 2,
  HELP = 3,
};

typedef enum OptionIndex OptionIndex;

struct AvaibleOption {
  const char *option;
  OptionIndex option_index;
};

typedef struct AvaibleOption AvaibleOption;

extern CreateState create_in_same_dir(char *dirpath, char **argv, int argc);
extern CreateState create_different_filepath(char **argv, int argc);
// create differents files in the same directories
extern CreateState create_different_dir(char **argv, int argc);
extern CreateState choose_option(char *option, char **argv, int argc);

extern bool is_dir_exist(const char *dirpath);
extern int count_args(int *count, char *argument, size_t argument_len);
extern bool extract_list(int list_len, char ***list, char *argument,
                         size_t argument_len);
extern bool is_arg_valid(const char *argument, size_t argument_len);
#endif // !CREATE_H
// EOF
