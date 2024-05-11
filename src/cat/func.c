#include "func.h"
#include <stdio.h>
#include <unistd.h>

int opt_flags(int c, char *argv[], struct for_flags *f) {
  int opt;
  int err = 0;
  while ((opt = getopt(c, argv, "+benstvET")) != -1) {
    switch (opt) {
    case 'b':
      f->f_b = 1;
      break;
    case 'E':
      f->f_E = 1;
      break;
    case 'n':
      f->f_n = 1;
      break;
    case 's':
      f->f_s = 1;
      break;
    case 'T':
      f->f_T = 1;
      break;
    case 'e':
      f->f_e = 1;
      break;
    case 't':
      f->f_t = 1;
      break;
    case 'v':
      f->f_v = 1;
      break;
    default:
      err = 1;
    }
  }
  return err;
}

void proc(struct for_flags *f, FILE *ms) {
  char curchar;
  char prevchar = 10;
  char prevprevchar = 9;
  int schet = 1;
  while ((curchar = fgetc(ms)) != EOF) {
    if (f->f_b == 1) { // flag_b
      if (curchar != 10 && prevchar == 10)
        output(1, schet++, 0, 0, 0);
    } else if (f->f_n == 1) { // flag_n
      if (prevchar == 10) {
        if (f->f_s == 1 && curchar == 10 && prevprevchar == 10) {
        } else {
          output(1, schet++, 0, 0, 0);
        }
      }
    }
    if (f->f_e == 1 || f->f_E == 1) { // flag_e
      if (curchar == 10) {
        if (f->f_s == 1 && curchar == 10 && prevchar == 10 &&
            prevprevchar == 10) {
        } else {
          output(0, 0, 36, 0, 0);
        }
      }
    }
    if (f->f_t == 1 || f->f_T == 1) { // flag_t
      if (curchar == 9) {
        output(0, 0, 94, 0, 0);
        output(0, 0, 73, 0, 0);
      }
    }
    if (f->f_e == 1 || f->f_t == 1 || f->f_v == 1) { // flag_v
      if (curchar >= 0 && curchar <= 31 && curchar != 9 && curchar != 10) {
        curchar += 64;
        output(0, 0, 94, 0, 0);
      } else if (curchar == 127) {
        curchar = 63;
        output(0, 0, 94, 0, 0);
      }
    }
    if ((curchar == 9 && (f->f_T == 1 || f->f_t == 1)) ||
        (f->f_s == 1 && curchar == 10 && prevchar == 10 &&
         prevprevchar == 10)) { // flag_s
    } else {
      output(0, 0, curchar, 0, 0);
    }
    prevprevchar = prevchar;
    prevchar = curchar;
  }
  schet = 1;
  prevchar = 10;
  prevprevchar = 9;
  fclose(ms);
}

void output(int f, int n, int symb, int c, char *argv[]) {
  switch (f) {
  case 0:разрешение экрана
    printf("%c", symb);
    break;
  case 1:
    printf("%6d\t", n);
    break;
  case 2:
    fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[c]);
    break;
  case 3:
    fprintf(stderr, "Too few arguments\n");
    break;
  case 4:
    fprintf(stderr, "usage: %s [-beEnstTv] [file ...]\n", argv[0]);
  }
}
