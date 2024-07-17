#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "iscreated.h"

void write_to_file(
    char *filepath,
    char **argv,
    int argc,
    int start, 
    char *text_to_repeat
) {
    if(access(filepath, F_OK) == 0) {
        fprintf(stderr, "!!!%s exist already!!!\n", filepath);
        return;
    }

    FILE *filepointer = fopen(filepath, "w+");

    for(int i = start; i < argc; ++i) {
        if(strcmp(argv[i], "") != 0) {
            fprintf(filepointer, "%s%s\n", text_to_repeat, argv[i]);
        }
    }

    fclose(filepointer);
}

void create_file(int argc, char **argv) {
    char *filepath = argv[1];
    char *text_to_repeat;

    if(access(filepath, F_OK) == 0) {
        fprintf(stderr, "!!!%s exist already!!!\n", filepath);
        return;
    } 

    if(argc > 3 && strcmp(argv[2], "import") == 0){
        const unsigned long LENGTH = strlen(argv[2]) + 2;
        text_to_repeat = malloc(LENGTH * sizeof(char));
        strncpy(text_to_repeat, argv[2], LENGTH);
        strncat(text_to_repeat, " ", LENGTH);
        write_to_file(filepath, argv, argc, 3, text_to_repeat);
        free(text_to_repeat);
    } else {
        text_to_repeat = "";
        write_to_file(filepath, argv, argc, 2, text_to_repeat);
    }

    if(access(filepath, F_OK) == 0) {
        printf("%s was successfully created!\n", filepath);
    } else {
        fprintf(stderr, "!!!%s was not created!!!\n", filepath);
    }
}

int main(int argc, char **argv) {
    if(argc > 1) {
        create_file(argc, argv);
    } else {
        fprintf(stderr, "!!!No filepath was given!!!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
