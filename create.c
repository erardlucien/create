#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <io.h>
#define F_OK 0
#define access _access
#else
#include <unistd.h>
#endif

void write_to_file(
    char *filename,
    char **argv,
    int argc,
    int start, 
    char *text_to_repeat
) {
    FILE *filepointer = fopen(filename, "w+");

    for(int i = start; i < argc; ++i) {
        if(i == start) {
            fprintf(filepointer, "%s%s", text_to_repeat, argv[i]);
        } else {
            fprintf(filepointer, "\n%s%s", text_to_repeat, argv[i]);
        }
    }

    fclose(filepointer);
}

int main(int argc, char **argv) {
    char *filename = "";
    char text_to_repeat[] = "";
    if(argc > 1) {
        filename = argv[1];
    } else {
        fprintf(stderr, "!!!No filename was given!!!");
        return EXIT_FAILURE;
    }

    if(argc > 2 && strcmp(argv[2], "import") == 0){
        const size_t LENGTH = strlen(argv[2]) + 1;
        strncat(text_to_repeat, argv[2], LENGTH);
        strncat(text_to_repeat, " ", LENGTH);
        write_to_file(filename, argv, argc, 3, text_to_repeat);
    } else {
        write_to_file(filename, argv, argc, 2, text_to_repeat);
    }

    if(access(filename, F_OK) == 0) {
        printf("%s was successfully created!\n", filename);
    } else {
        fprintf(stderr, "!!!%s was not created!!!\n", filename);
    }

    return EXIT_SUCCESS;
}
