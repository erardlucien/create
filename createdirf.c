#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "create_file.h"

enum CreateState {
    COULD_NOT_CREATE = -6,
    NO_FILENAME_WAS_GIVEN = -5,
    NOT_VALID = -4,
    COULD_NOT_SET_PATH = -3,
    PATH_ALREADY_EXIST = -2,
    PATH_TOO_LONG = -1,
    CREATE_SUCCESS = 0,
};

typedef enum CreateState CreateState;

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

CreateState create_directory_from(char *directory_path) {
    CreateState result = CREATE_SUCCESS;

    if(control(directory_path) != VALID_ARGUMENT_LENGTH) {
        fprintf(stderr, "!!!the filepath is too big!!!\n");
        fprintf(stderr, "!!!The length of the path must be smaller than %d!!!\n", PATH_MAX);
        return PATH_TOO_LONG;
    }

    #ifdef __linux__
        if( strlen(directory_path) == 0 || strcmp(directory_path, ".") == 0 ) {
            return result;
        }
        if(existed_already(directory_path) && strcmp(directory_path, ".") != 0 ) {
            printf("!!!%s exist already!!!\n", directory_path);
            return PATH_ALREADY_EXIST;
        }
        result = mkdir(directory_path, 0700);
    #else
        if( strlen(directory_path) == 0 || strcmp(directory_path, ".") == 0 ) {
            return result;
        }
        if(existed_already(directory_path) && strcmp(directory_path, ".") != 0 ) {
            printf("!!!%s exist already!!!\n", directory_path);
            return PATH_ALREADY_EXIST;
        }
        result = _mkdir(directory_path);
    #endif

    if(result < 0) {
        fprintf(stderr, "!!!Could not create %s!!!\n", directory_path);
        result = COULD_NOT_CREATE;
    }

    return result;
}

CreateState create_directories_from(char *filepath, char character) {
    int counter = 0;
    int from = 0;
    char *directory = "";
    CreateState result = CREATE_SUCCESS;

    #ifdef __linux__
    if( !is_valid('/', filepath) ) {
        fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        fprintf(stderr, "%s\nis not valid\n", filepath);
        fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        return NOT_VALID;
    }
    #else 
    if( !is_valid('\\', filepath) ) {
        fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        fprintf(stderr, "%s\nis not valid\n", filepath);
        fprintf(stderr, "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        return NOT_VALID;
    }
    #endif

    const int FILEPATH_LENGTH = strlen(filepath);
    for(int i = 0; i < FILEPATH_LENGTH && counter < count(character, filepath); ++i) {
        if(filepath[i] == character) {
            const int LENGTH = indexOfInRange(character, filepath, from, FILEPATH_LENGTH) + 1;
            directory = (char *)malloc(LENGTH * sizeof(char));

            if(directory == NULL) {
                fprintf(stderr, "!!!Could not set the path!!!\n");
            }

            directory[LENGTH - 1] = '\0';
            strncpy(directory, filepath, LENGTH - 1);
            result = create_directory_from(directory);

            if(result == COULD_NOT_CREATE) {
                return result;
            }

            ++counter;
            from = i + 1;
        }
    }

    if(FILEPATH_LENGTH == strlen(directory) + 1) {
        result = NO_FILENAME_WAS_GIVEN;
    }

    return result;
}


void create_file_and_dir(int argc, char **argv) {

    for(int i = 1; i < argc; ++i) {
        CreateState result = CREATE_SUCCESS;

        #ifdef __linux__
            result = create_directories_from(argv[i], '/');
        #else
            result = create_directories_from(argv[i], '\\');
        #endif

        if(result == NO_FILENAME_WAS_GIVEN) {
            fprintf(stderr, "!!!No filename was given!!!\n");
        } else if(result != CREATE_SUCCESS && result != PATH_ALREADY_EXIST) {
            fprintf(stderr, "!!!A failure has occurred!!!\n");
        } else {
            create_file(argv[i]);
        }
    }
}

int main(int argc, char **argv) {

    if(argc < 2) {
        fprintf(stderr, "!!!No FILEPATH was given!!!\n");
        printf("Write cdf -h to see, how use this command\n");
        return EXIT_FAILURE;
    } if(argc == 2 && strncmp(argv[1], HELP_TEXT, HELP_MAX_STR) == 0) {
        printf("Create more than one file and  the parent-directories.\n");
        printf("If the parents-directories are already created, then only the files will be created.\n");
        printf("Use it like this:\n");
        printf("cdf -h for help");
        printf("cdf filepath\n");
        printf("Or\n");
        printf("cdf filepath1 filepath2 ...\n");
        printf("... stand for more filepath could be given.\n");
        return EXIT_SUCCESS; 
    } 

    create_file_and_dir(argc, argv);

    return EXIT_SUCCESS;
}
