#include "s21_grep.h"
#include <stdio.h>

int main(int argc, char **argv) {
  struct for_flags flags = {0};
  char pattstr[4096];  // объявление массива для строки шаблонов
  if (argc < 3) {
    output(3, 0, 0, 0, 0, 0, 0, 0);
  } else if (options_flags(argc, argv, &flags, pattstr) != 0) {
    output(4, 0, 0, 0, argv, 0, 0, 0);
  } else {
    proc(argc, argv, &flags, pattstr);
  }
  return 0;
}
