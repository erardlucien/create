#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iscreated.h"add

void write_to_file(
    char *filename,
    char **argv,
    int argc,
    int start, 
    char *text_to_repeat
) {
    if(access(filename, F_OK) == 0) {
        fprintf(stderr, "!!!%s exist already!!!\n", filename);
        return;
    }

    FILE *filepointer = fopen(filename, "w+");

    for(int i = start; i < argc; ++i) {
        if(strcmp(argv[i], "") != 0) {
            fprintf(filepointer, "%s%s\n", text_to_repeat, argv[i]);
        }
    }

    fclose(filepointer);
}

void create_file(int argc, char **argv) {
    char *filename = argv[1];
    char *text_to_repeat;

    if(access(filename, F_OK) == 0) {
        fprintf(stderr, "!!!%s exist already!!!\n", filename);
        return;
    } 

    if(argc > 3 && strcmp(argv[2], "import") == 0){
        const unsigned long LENGTH = strlen(argv[2]) + 2;
        text_to_repeat = malloc(LENGTH * sizeof(char));
        strncpy(text_to_repeat, argv[2], LENGTH);
        strncat(text_to_repeat, " ", LENGTH);
        write_to_file(filename, argv, argc, 3, text_to_repeat);
        free(text_to_repeat);
    } else {
        text_to_repeat = "";
        write_to_file(filename, argv, argc, 2, text_to_repeat);
    }

    if(access(filename, F_OK) == 0) {
        printf("%s was successfully created!\n", filename);
    } else {
        fprintf(stderr, "!!!%s was not created!!!\n", filename);
    }

    if(strstr(filename, "py") != NULL) {
        char **argv2 = (char **) malloc(argc * sizeof(*argv));
        for(int i = 0; i < argc; ++i) {
            if(i == 1) {
                argv2[1] = "requirements.txt";
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
        fprintf(stderr, "!!!No filename was given!!!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
