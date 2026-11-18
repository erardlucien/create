#ifndef CNF_H
#define CNF_H
// standart library
// project's header
#include "../../../common/include/files.h"

enum OptionIndex {
  INVALID_OPTION = -1,
  DIFFERENT_FILEPATH = 0,
  SAME_DIR = 1,
  DIFFERENT_DIRS = 2,
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

#endif // !CNF_H
// EOF
