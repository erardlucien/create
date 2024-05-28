#include <stdio.h>
#include <stdlib.h>
#include "iscreated.h"

int main(int argc, char **argv) {
    if(argc < 2) {
<<<<<<< HEAD
        fprintf(stderr, "!!!No filename was given!!!");
        return EXIT_FAILURE;
    }

    for(int i = 0; i < argc; ++i) {
        const char *FILENAME = argv[i];
=======
        fprintf(stderr, "!!!No filename was given!!!\n");
        return EXIT_FAILURE;
    }

    for(int i = 1; i < argc; ++i) {
        const char *FILENAME = argv[i];

        if(access(FILENAME, F_OK) == 0) {
            fprintf(stderr, "!!!%s exist already!!!\n", FILENAME);
            return EXIT_FAILURE;
        }
        
>>>>>>> refs/remotes/origin/main
        FILE *FILEPOINTER = fopen(FILENAME, "w+");
        fclose(FILEPOINTER);

        if(access(FILENAME, F_OK) == 0) {
            printf("%s was successfully created!\n", FILENAME);
        } else {
            fprintf(stderr, "!!!%s was not created!!!\n", FILENAME);
        }

    }

    return EXIT_SUCCESS;
<<<<<<< HEAD
}
=======
}
>>>>>>> refs/remotes/origin/main
