#ifndef CREATE_FILE_H
#define CREATE_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include "iscreated.h"

#define HELP_TEXT "-h"
#define HELP_MAX_STR 3

typedef enum {
  VALID_ARGUMENT_LENGTH = 0,
  INVALID_ARGUMENT_LENGTH = -1,
} ArgumentLengthValidation;

void create_file(const char *FILEPATH);
ArgumentLengthValidation control(char *filepath);

#endif
