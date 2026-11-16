// standart library
#include <unistd.h>
// header-project
#include "../include/test_create_file.h"

bool is_file_created(char *filepath) { return access(filepath, F_OK) == 0; }
