// standart-libary
#include <stdlib.h>
// custom-libary
#include "../include/files.h"

const char *ALLOWED_TYPE_FILE[] = {
    ".c",  ".html", ".cpp", ".h",   ".rs",  ".txt", ".md",   ".css",
    ".py", ".js",   ".hpp", ".csv", ".tex", ".xml", ".json", ".mk"};
const size_t ALLOWED_TYPE_FILE_LEN =
    sizeof(ALLOWED_TYPE_FILE) / sizeof(ALLOWED_TYPE_FILE[0]);
