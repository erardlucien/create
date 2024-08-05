#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iscreated.h"

void printSuccessMsg(char *filepath) {
    printf("%s was succefully updated!\n", filepath);
}

int update_file(
    char *filepath,
    char **argv, 
    int argc,
    int start,
    char *text_to_repeat
) {
    
    if(access(filepath, F_OK) != 0) {
        fprintf(stderr, "!!!%s doesn't exist!!!\n", filepath);
        return EXIT_FAILURE;
    }

    FILE *filepointer = fopen(filepath, "a");

    if(filepointer == NULL) {
        fprintf(stderr, "!!!A error occured!!!\n");
        return EXIT_FAILURE;
    }

    for(int i = start; i < argc; ++i) {
        fprintf(filepointer, "%s%s\n", text_to_repeat, argv[i]);
    }

    fclose(filepointer);

    printSuccessMsg(filepath);

    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    char *filepath = "";
    char *text_to_repeat;
    int result = 0;

    if(argc > 1) {
        filepath = argv[1];
    } else {
        fprintf(stderr, "!!!No filepath was given!!!\n");
        return EXIT_FAILURE;
    }

    if(argc > 3 && strcmp(argv[2], "import") == 0){
        const unsigned long LENGTH = strlen(argv[2]) + 2;
        text_to_repeat = malloc(LENGTH * sizeof(char));
        strncpy(text_to_repeat, argv[2], LENGTH);
        strncat(text_to_repeat, " ", LENGTH);
        result = update_file(filepath, argv, argc, 3, text_to_repeat);
        free(text_to_repeat);
    } else {
        text_to_repeat = "";
        result = update_file(filepath, argv, argc, 2, text_to_repeat);
    }

    return result;
}