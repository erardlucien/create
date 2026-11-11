#include "create_file.h"
#include <string.h>
#include <ctype.h>

void create_file(const char *FILEPATH) {

    if(check_first_char(FILEPATH) == INVALID_FCHARACTER) {
        fprintf(stderr, "!!!Invalid first character!!!\n");
        return;
    }
   
    if(control(FILEPATH) != VALID_ARGUMENT_LENGTH) {
       fprintf(stderr, "!!!the filepath is too big!!!\n");
       fprintf(stderr, "!!!The length of the path must be smaller than %d!!!\n", PATH_MAX);
       return;
    }

    if(access(FILEPATH, F_OK) == 0) {
        printf("!!!%s exist already!!!\n", FILEPATH);
        return;
    }

    FILE *FILEPOINTER = fopen(FILEPATH, "w");

    if(FILEPOINTER == NULL) {
        fprintf(stderr, "!!!%s could not be created!!!\n", FILEPATH);
        return;
    }

    fclose(FILEPOINTER);

    if(access(FILEPATH, F_OK) == 0) {
        printf("%s was successfully created!\n", FILEPATH);
    } else {
        fprintf(stderr, "!!!%s was not created!!!\n", FILEPATH);
    }
}

int count(const char character, const char *text) {
    int result = 0;
    for(int i = 0; i < strlen(text); ++i) {
        if(text[i] == character) {
            ++result;
        }
    }

    return result;
}


bool is_valid(const char character, const char *filepath) {

    if(count('.', filepath) > 2 && filepath[0] == '.') {
        return false;
    }

    for(int i = 0; i < strlen(filepath); ++i) {
        if(
            !isalnum(filepath[i])
            && filepath[i] != character
            && filepath[i] != '.'
            && filepath[i] != '_'
            && filepath[i] != '-'
        ) {
            return false;
        }
    }

    return true;
}


// Check if the first character is valid.
CharacterValidation check_first_char(const char *text) {
    if(text[0] == '-' && strncmp(text, HELP_TEXT, HELP_TEXT_LEN) != 0 &&
    strncmp(text, SAME_DIRECTORY, SAME_DIRECTORY_LEN) != 0 ) {
        return INVALID_FCHARACTER;
    }

    if(!isalpha(text[0]) && text[0] != '.' && text[0] != '_' && text[0] != '/') {
        return INVALID_FCHARACTER;
    }

    return VALID_FCHARACTER;
}

ArgumentLengthValidation control(const char *filepath) {
    int actual_len = strnlen(filepath, PATH_MAX);
    if(actual_len > PATH_MAX) {
        return INVALID_ARGUMENT_LENGTH;
    }
  return VALID_ARGUMENT_LENGTH;
}

