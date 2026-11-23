#ifndef CREATE_H
#define CREATE_H
// standart library
#include <stdbool.h>
// project's header
#include "../../../common/include/files.h"

#define SEP_STR "/"
#define SEP_CHAR '/'
#define POINT '.'
#define HYPHEN '-'
#define UNDERSCORE '_'
#define TILDE '~'
#define QUESTION_MARK '?'
#define FILE_MARKER "--files"
#define DF_OPTION "--df"
#define DD_OPTION "--dd"
#define SD_OPTION "--sd"
#define HELP_OPTION "--help"

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

extern CreateState create_in_same_dir(char **argv, int argc);
extern CreateState create_different_filepath(char **argv, int argc);
extern CreateState create_different_dir(char **argv, int argc);
extern CreateState choose_option(char *option, char **argv, int argc);
extern CreateState count_elem(int *pcounter, char *start_deliminator,
                              int *pindex_st_deliminator, char *end_deliminator,
                              char **argv, int argc);
extern CreateState gen_list(int counter, char ***list, int index_st_deliminator,
                            char **argv, int argc);

extern void free_vector(char ***pvector, int n_row);
extern bool is_arg_valid(const char *argument, size_t argument_len);
#endif // !CREATE_H
// EOF
