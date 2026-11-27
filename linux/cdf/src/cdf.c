// standart library
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
// project's header
#include "../../common/include/create.h"

#define LEN(text) (text) != NULL ? sizeof(text) / sizeof(text[0]) : 1

static void show_help(void);

int main(int argc, char **argv) {
  int option_index = 1;

  CreateState state = choose_option(argv[option_index], argv, argc);
  switch (state) {
  case CREATE_OK:
    break;
  case CREATE_HELP:
    show_help();
    break;
  case CREATE_INVALID_OPTION:
    printf("Write cdf --help to see how use this cdf\n");
    break;
  default:
    fprintf(stderr, "[ERROR] %d", state);
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

static void show_help() {
  const char *msg[] = {
      "You should use cdf like this cdf --option ![args]\n",
      "![args] means depends from option\n",
      "option can have 4 options:\n",
      "--help show information how use this cdf\n",
      "*Note:* here args must not be given.\n",
      "--df create files in diffrent path\n",
      "args is here the diffrent filepath\n.",
      "*Example:* cdf --df /home/dummy/Docs/food.txt",
      "/home/dummy/Downloads/secret.xml\n",
      "--dd create differents files in differents directories\n",
      "*Example:* cdf --dd /home/dummy/Documents/ --files log.txt main.cpp ",
      "--dd ... --files ...\n",
      "... meant here other arguments\n",
      "*Note:* : must be given here or else no file will be created\n",
      "--sd create more files in same directory\n",
      "*Example:* cdf --sd /home/dummy/Docs/ --files food.txt data.csv\n",
  };

  size_t len = LEN(msg);

  for (size_t i = 0; i < len; ++i) {
    printf("%s", msg[i]);
  }
}
