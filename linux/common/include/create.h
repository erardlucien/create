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
#define PD_OPTION "--pd"
#define CHILD_MARKER "--subd"

enum OptionIndex {
  INVALID_OPTION = -1,
  DIFFERENT_FILEPATH = 0,
  SAME_DIR = 1,
  DIFFERENT_DIRS = 2,
  HELP = 3,
  PARENT = 4,
};

typedef enum OptionIndex OptionIndex;

struct AvaibleOption {
  const char *option;
  OptionIndex option_index;
};

typedef struct AvaibleOption AvaibleOption;

struct ArgumentControlLer {
  char **parent_marker;
  // index of option marker;
  int pmarker_i;
  int parent_num;
  char **child_marker;
  // index of subdirector
  int child_i;
  int child_num;
  char **files_marker;
  // index of files_marker;
  int fmarker_i;
  int files_num;
  int argc;
};

typedef struct ArgumentControlLer ArgumentControlLer;

struct MarkCounter {
  int pmarker_counter;
  int cmarker_counter;
  int fmarker_counter;
};

typedef struct MarkCounter MarkCounter;

struct DataCounter {
  char **pmarker;
  int counter;
  struct DataCounter *next;
};

typedef struct DataCounter DataCounter;

extern CreateState create_in_same_dir(char **argv, int argc);
extern CreateState create_different_filepath(char **argv, int argc);
extern CreateState create_different_dir(char **argv, int argc);
extern CreateState choose_option(char *option, char **argv, int argc);
extern CreateState find_index(ArgumentControlLer *pcontroller, char *option,
                              char **argv, int argc);
extern void find_fmarker_i(int *index, char **argv, int argc);
extern void find_pmarker_i(int *index, char **argv, int argc, char *option);
extern void count_marker(MarkCounter *pcounter, char **argv, int argc);
extern CreateState validate_marker(MarkCounter *pcounter, char *option,
                                   int option_counter);
extern void count_option(int *pcounter, char *option, char **argv, int argc);
extern void find_marker(DataCounter *pcounter, char *marker, char **argv,
                        int argc);
extern void count_data(DataCounter *pcounter, char **not_allowed,
                       int not_allowed_len, char *end_marker, char **argv,
                       int argc);
extern CreateState validate_data_count(DataCounter *pcounter);
extern CreateState count_num(ArgumentControlLer *pcontroller);

extern bool is_arg_valid(const char *argument, size_t argument_len);
#endif // !CREATE_H
// EOF
