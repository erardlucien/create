#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
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

bool is_valid(char character, char *filepath) {

    if(count('.', filepath) != 1) {
        return false;
    }

    for(int i = 0; i < strlen(filepath); ++i) {
        if(
            !isalnum(filepath[i])
            && filepath[i] != character
            && filepath[i] != '.'
        ) {
            return false;
        }
    }

    return true;
}

bool existed_already(char *directory) {
    struct stat sb;
    return stat(directory, &sb) == 0 && S_ISDIR(sb.st_mode);
}

int create_directory_from(char *directory_path) {
    int result = 0;

    #ifdef __linux__
        if(strlen(directory_path) == 0) {
            return 0;
        }
        if(existed_already(directory_path)) {
            printf("!!!%s exist already!!!\n", directory_path);
            return result;
        }
        result = mkdir(directory_path, 0700);
    #else
        if(strlen(directory) == 0) {
            return 0;
        }
        if(existed_already(directory)) {
            printf("!!!%s exist already!!!\n", directory);
            return result;
        }
        result = _mkdir(directory);
    #endif

    return result;
}

int create_directories_from(char *filepath, char character) {
    int counter = 0;
    char *directory = "";
    int result = 0;

    if( !( is_valid('/', filepath) || is_valid('\\', filepath) ) ) {
        fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        fprintf(stderr, "%s\nis not valid\n", filepath);
        fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        return -2;
    }

    for(int i = 0; i < strlen(filepath) && counter < count(character, filepath); ++i) {
        const char *TMP = directory;
        const int LENGTH = strlen(directory) + 2;
        directory = (char *)malloc(LENGTH * sizeof(char));
        strcpy(directory, TMP);
        directory[LENGTH - 2] = filepath[i];
        if(filepath[i] == character) {
            directory[LENGTH - 1] = '\0';
            result = create_directory_from(directory);
            ++counter;
        }

        if(result < 0) {
            return result;
        }
    }

    return result;
}


void create_file_and_dir(int argc, char **argv) {

    for(int i = 1; i < argc; ++i) {
        int result = 0;

        #ifdef __linux__
            result = create_directories_from(argv[i], '/');
        #else
            result = create_directories_from(argv[i], '\\');
        #endif

        if(result < 0) {
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