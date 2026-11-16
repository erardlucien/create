// standart library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// header project
#include "../../../common/include/files.h"

int main(int argc, char **argv) {
  for (int i = 1; i < argc; ++i) {
    CreateState state = create_file(argv[i], strlen(argv[i]));
    if (state != CREATE_OK) {
      fprintf(stderr, "Error: %d\n", state);
    }
  }
  return EXIT_SUCCESS;
}
