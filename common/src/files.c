// standart-libary
#include <stdlib.h>
// custom-libary
#include "../include/files.h"

const char ALLOWED_SPECIAL_CHARACTER[] = {':', '.', '/', '\\', ' ', '-'};
const size_t ALLOWED_SPECIAL_CHARACTER_LEN =
    sizeof(ALLOWED_SPECIAL_CHARACTER) / sizeof(ALLOWED_SPECIAL_CHARACTER[0]);
const char *ALLOWED_TYPE_FILE[] = {".c",   ".html", ".cpp", ".h",   ".rs",
                                   ".txt", ".md",   ".css", ".py",  ".js",
                                   ".hpp", ".csv",  ".tex", ".xml", ".json"};
const size_t ALLOWED_TYPE_FILE_LEN =
    sizeof(ALLOWED_TYPE_FILE) / sizeof(ALLOWED_TYPE_FILE[0]);
