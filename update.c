#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "iscreated.h"

void update_file(
    char *filepath,
    char **argv, 
    int argc,
    int start,
    char *text_to_repeat
) {
    struct stat st;

    if(stat(filepath, &st) < 0) {
        fprintf(stderr, "!!!%s is not a valid path!!!\n", filepath);
        return;
    }
    
    if(access(filepath, F_OK) != 0) {
        fprintf(stderr, "!!!%s doesn't exist!!!\n", filepath);
        return;
    }

    FILE *filepointer = fopen(filepath, "a");

    if(filepointer == NULL) {
        printf("!!!%s doesn't exist!!!", filepath);
    }

    for(int i = start; i < argc; ++i) {
        fprintf(filepointer, "%s%s\n", text_to_repeat, argv[i]);
    }

    fclose(filepointer);
}

void printSuccessMsg(char *filepath) {
    printf("%s was succefully updated!\n", filepath);
}

int main(int argc, char **argv) {
    char *filepath = "";
    char *text_to_repeat;

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
        update_file(filepath, argv, argc, 3, text_to_repeat);
        free(text_to_repeat);
        printSuccessMsg(filepath);
    } else {
        text_to_repeat = "";
        update_file(filepath, argv, argc, 2, text_to_repeat);
        printSuccessMsg(filepath);
    }

    return EXIT_SUCCESS;
}