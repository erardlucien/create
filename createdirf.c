#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "create_file.h"

#ifndef __linux__
#include <direct.h>
#endif

int indexOfInRange(char character, char *text, int from, int to) {
    for(int i = from; i < to; i++) {
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

bool is_valid(char character, char *filepath) {

    if(count('.', filepath) != 2 && filepath[0] == '.') {
        return false;
    }

    for(int i = 0; i < strlen(filepath); ++i) {
        if(
            !isalnum(filepath[i])
            && filepath[i] != character
            && filepath[i] != '.'
            && filepath[i] != '_'
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
            return result;
        }
        if(existed_already(directory_path) && !strcmp(directory_path, ".") == 0) {
            printf("!!!%s exist already!!!\n", directory_path);
            return result;
        }
        result = mkdir(directory_path, 0700);
    #else
        if(strlen(directory_path) == 0) {
            return result;
        }
        if(existed_already(directory_path) && !strcmp(directory_path, ".") == 0) {
            printf("!!!%s exist already!!!\n", directory_path);
            return result;
        }
        result = _mkdir(directory_path);
    #endif

    return result;
}

int create_directories_from(char *filepath, char character) {
    int counter = 0;
    int from = 0;
    char *directory = "";
    int result = 0;

    if( !( is_valid('/', filepath) || is_valid('\\', filepath) ) ) {
        fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        fprintf(stderr, "%s\nis not valid\n", filepath);
        fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        return -2;
    }

    const int FILEPATH_LENGTH = strlen(filepath);
    for(int i = 0; i < FILEPATH_LENGTH && counter < count(character, filepath); ++i) {
        if(filepath[i] == character) {
            const int LENGTH = indexOfInRange(character, filepath, from, FILEPATH_LENGTH) + 1;
            directory = (char *)malloc(LENGTH * sizeof(char));
            directory[LENGTH - 1] = '\0';
            strncpy(directory, filepath, LENGTH - 1);
            result = create_directory_from(directory);
            ++counter;
            from = i + 1;
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