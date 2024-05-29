#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iscreated.h"

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
        fprintf(filepointer, "%s%s\n", text_to_repeat, argv[i]);
    }

    fclose(filepointer);
}

int main(int argc, char **argv) {
    char *filename = "";
    char *text_to_repeat;
    
    if(argc > 1) {
        filename = argv[1];
    } else {
        fprintf(stderr, "!!!No filename was given!!!\n");
        return EXIT_FAILURE;
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

    return EXIT_SUCCESS;
}
