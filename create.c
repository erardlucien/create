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

size_t length_before_filename(char *filepath) {
    size_t length = strlen(filepath) + 1;
    size_t i = strlen(filepath) - 1;
    while(
        i < length - 1
        && filepath[i] != '/'
        && filepath[i] != '\\'
    ) {
        --i;
    }

    return i + 1;
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

    if(
        argc > 2
        && strstr(filepath, "py") != NULL
        && strcmp(argv[2], "import") == 0
    ) {
        char **argv2 = (char **) malloc(argc * sizeof(*argv));
        char *filename = "requirements.txt";
        size_t length = length_before_filename(argv[1]);
        for(int i = 0; i < argc; ++i) {
            if(i == 1) {
                argv2[1] = (char *) malloc( ( abs( length + strlen(filename) ) + 1) * sizeof(char));
                strncpy(argv2[1], argv[1], length);
                strcat(argv2[1], filename);
            } else if(i == 2 && strcmp(argv[i], "import") == 0) {
                argv2[2] = "";
            } else {
                argv2[i] = (char *) malloc(sizeof(argv));
                strcpy(argv2[i], argv[i]);
            }
        }
        create_file(argc, argv2);
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
