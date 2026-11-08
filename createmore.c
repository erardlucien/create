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

    if(argc == 2 && strncmp(argv[1], HELP_TEXT, HELP_MAX_STR) == 0) {
        printf("Create more than one file.\n");
        printf("The parents-directories needs to exist.\n");
        printf("Use it like this:\n");
        printf("cfmore -h for help");
        printf("cfmore filepath\n");
        printf("Or\n");
        printf("cfmore filepath1 filepath2 ...\n");
        printf("... stand for more filepath could be given.\n");
        return EXIT_SUCCESS; 
    }

    create_more_file(argv, argc);

    return EXIT_SUCCESS;
}
