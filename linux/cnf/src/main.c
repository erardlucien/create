// standart library
#include <stdio.h>
#include <stdlib.h>
// header project
#include "../../../common/include/files.h"
#include "../include/cnf.h"

int main(int argc, char **argv) {
  CreateState state = create_different_dir(argv + 1, argc - 1);
  if (state != CREATE_OK) {
    printf("[ERROR] - %d", state);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
