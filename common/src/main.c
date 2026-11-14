// standart library
#include <stdio.h>
#include <string.h>
// my header
#include "../include/files.h"

int main(void) {
  char filename[] = "main.cpp";
  printf("%d\n", validate_filename(filename, strlen(filename)));
  return 0;
}
