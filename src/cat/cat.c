#include "func.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  struct for_flags flags = {0};
  FILE *mystream;
  if (argc <= 1) {
    output(3, 0, 0, 0, 0);
  } else {
    if (opt_flags(argc, argv, &flags) == 0) {
      for (int cfiles = optind; cfiles < argc; cfiles++) {
        if ((mystream = fopen(argv[cfiles], "r")) != NULL) {
          proc(&flags, mystream);
        } else {
          output(2, 0, 0, cfiles, argv);
          continue;
        }
      }
    } else {
      output(4, 0, 0, 0, argv);
    }
  }
  return 0;
}
