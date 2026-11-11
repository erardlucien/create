#include "create_file.h"
#include <string.h>

void create_more_file(char **argv, int argc) {
    for(int i = 1; i < argc; ++i) {
        const char *FILEPATH = argv[i];

        create_file(FILEPATH);
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
        printf("When you need to create more files in the same directory:\n");
        printf("cfmore -sd  directory-path filename1 filename2 ... filenameN\n");
        
        return EXIT_SUCCESS; 
    }

   
    if(strncmp(argv[1], SAME_DIRECTORY, SAME_DIRECTORY_LEN) == 0) {
        printf("This option must be reimplemented. It was removed because of some failure in the implementation. For now is no longer avaible\n");
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
