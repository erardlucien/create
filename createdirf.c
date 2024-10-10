#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include "create_file.h"

int indexOf(char character, char *text) {
    for(int i = strlen(text); i > -1; --i) {
        if(text[i] == character) {
            return i;
        }
    }

    return -1;
}

int count(char character, char *text) {
    int result = 0;
    for(int i = 0; i < strlen(text); ++i) {
        if(text[i] == character) {
            ++result;
        }
    }

    return result;
}

char *retrieve_path_directory_in(char character, char *filepath) {
    int index = indexOf(character, filepath); 

    if(index == -1) {
        return "";
    }

    int length = index + 2;
    char *result = (char *)malloc(length * sizeof(char));

    for(int i = 0; i < length - 1; ++i) {
        result[i] = filepath[i];
    }

    result[length - 1] = '\0';

    return result;
}

bool existed_already(char *directory) {
    struct stat sb;
    return stat(directory, &sb) == 0 && S_ISDIR(sb.st_mode);
}

int create_directory_from(char *filepath) {
    int result = 0;

    #ifdef __linux__
        char * directory = retrieve_path_directory_in('/', filepath);
        if(existed_already(directory)) {
            printf("!!!%s exist already!!!\n", directory);
            return result;
        }
        result = mkdir(directory, 0700);
    #else
        char * directory = retrieve_path_directory_in('\\', filepath);
        if(existed_already(directory)) {
            printf("!!!%s exist already!!!\n", directory);
            return result;
        }
        result = _mkdir(directory, filepath));
    #endif

    return result;
}

void create_file_and_dir(int argc, char **argv) {

    for(int i = 1; i < argc; ++i) {
        const int RESULT = create_directory_from(argv[i]);
        if(RESULT < 0) {
            fprintf(stderr, "!!!A failure has occurred!!!\n");
        } else {
            create_file(argv[i]);
        }
    }
}

int main(int argc, char **argv) {
    
    if(argc < 2) {
        fprintf(stderr, "!!!No FILEPATH was given!!!");
        return EXIT_FAILURE;
    }

    create_file_and_dir(argc, argv);

    return EXIT_SUCCESS;
}