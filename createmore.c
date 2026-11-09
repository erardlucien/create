#include "create_file.h"
#include <string.h>

void create_more_file(char **argv, int argc) {
    for(int i = 1; i < argc; ++i) {
        const char *FILEPATH = argv[i];

        create_file(FILEPATH);
    }
}

void create_more_file_same_dir(char **argv, int argc, char *directory) {
    char directory_cpy[PATH_MAX + 1] = "";
    strncpy(directory_cpy, directory, PATH_MAX);

   
    #ifdef __linux__
       strncat(directory_cpy, "/", PATH_MAX);
    #else
       strncat(directory_cpy, "\\", PATH_MAX);
    #endif



    for(int i = 3; i < argc; ++i) {
        char filepath[PATH_MAX + 1] = "";
        strncpy(filepath, directory_cpy, PATH_MAX);
        char *ptr = strncat(filepath, argv[i], PATH_MAX);
        
        create_file(filepath);
    }
}

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "!!!No FILEPATH was given!!!\n");
        printf("Write cfmore -h to see, how use this command\n");
        return EXIT_FAILURE;
    }

    if(argc == 2 && strncmp(argv[1], HELP_TEXT, HELP_TEXT_LEN) == 0) {
        printf("Create more than one file.\n");
        printf("The parents-directories needs to exist.\n");
        printf("Use it like this:\n");
        printf("cfmore -h for help");
        printf("cfmore filepath\n");
        printf("Or\n");
        printf("cfmore filepath1 filepath2 ... filenameN\n");
        printf("When you which to create more files in the same directory:\n");
        printf("cfmore -sd  directory-path filename1 filename2 ... filenameN\n");
        
        return EXIT_SUCCESS; 
    }

   
    if(strncmp(argv[1], SAME_DIRECTORY, SAME_DIRECTORY_LEN) == 0) {
         bool valid = false;
         #ifdef __linux__
              valid = is_valid('/', argv[1]);
         #else
              valid = is_valid('\\', argv[1]);
         #endif
        if(!valid) {
             fprintf(stderr, "!!!The directory-path is not valid!!!\n");
             printf("Write cdf -h to see, how use this command\n");
             return EXIT_FAILURE;
        }
        create_more_file_same_dir(argv, argc, argv[2]);
        return EXIT_SUCCESS;
    }

   if(argv[1][0] == '-') {
        printf("!!!Invalid arguments!!!\n");
        printf("Write cfmore -h to see, how use this command\n");
        return EXIT_FAILURE;
    }

    create_more_file(argv, argc);

    return EXIT_SUCCESS;
}
