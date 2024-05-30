#include <stdio.h>
#include <stdlib.h>
#include "iscreated.h"

void create_file(const char *FILENAME) {
    FILE *FILEPOINTER = fopen(FILENAME, "w+");
    fclose(FILEPOINTER);

    if(access(FILENAME, F_OK) == 0) {
        printf("%s was successfully created!\n", FILENAME);
    } else {
        fprintf(stderr, "!!!%s was not created!!!\n", FILENAME);
    }
}

void create_more_file(char **argv, int argc) {
    for(int i = 1; i < argc; ++i) {
        const char *FILENAME = argv[i];

        if(access(FILENAME, F_OK) == 0) {
            fprintf(stderr, "!!!%s exist already!!!\n", FILENAME);
        } else {
            create_file(FILENAME);
        }
    }
}

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "!!!No filename was given!!!");
        return EXIT_FAILURE;
    }

    create_more_file(argv, argc);

    return EXIT_SUCCESS;
}
