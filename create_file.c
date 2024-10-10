#include "create_file.h"

void create_file(const char *FILEPATH) {

    if(access(FILEPATH, F_OK) == 0) {
        printf("!!!%s exist already!!!\n", FILEPATH);
        return;
    }

    FILE *FILEPOINTER = fopen(FILEPATH, "w+");

    if(FILEPOINTER == NULL) {
        fprintf(stderr, "!!!A error occured!!!\n");
        return;
    }

    fclose(FILEPOINTER);

    if(access(FILEPATH, F_OK) == 0) {
        printf("%s was successfully created!\n", FILEPATH);
    } else {
        fprintf(stderr, "!!!%s was not created!!!\n", FILEPATH);
    }
}