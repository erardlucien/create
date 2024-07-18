#include <stdio.h>
#include <stdlib.h>
#include "iscreated.h"

void create_file(const char *FILEPATH) {

    FILE *FILEPOINTER = fopen(FILEPATH, "w+");

    if(FILEPOINTER == NULL) {
        fprintf(stderr, "!!!%s is not a valid path!!!\n", FILEPATH);
        return;
    }

    fclose(FILEPOINTER);

    if(access(FILEPATH, F_OK) == 0) {
        printf("%s was successfully created!\n", FILEPATH);
    } else {
        fprintf(stderr, "!!!%s was not created!!!\n", FILEPATH);
    }
}

void create_more_file(char **argv, int argc) {
    for(int i = 1; i < argc; ++i) {
        const char *FILEPATH = argv[i];

        if(access(FILEPATH, F_OK) == 0) {
            fprintf(stderr, "!!!%s exist already!!!\n", FILEPATH);
        } else {
            create_file(FILEPATH);
        }
    }
}

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "!!!No FILEPATH was given!!!");
        return EXIT_FAILURE;
    }

    create_more_file(argv, argc);

    return EXIT_SUCCESS;
}
