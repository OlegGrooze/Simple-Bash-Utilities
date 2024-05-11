#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#include <stdio.h>

struct for_flags {
  int f_e, f_i, f_v, f_c, f_l, f_n, f_h, f_s, f_f, f_o;
};
int options_flags(int ac, char **av, struct for_flags *f, char *ps);
int openFile(char *filename);
int getString(char *strarr);
void formOptArg(char *ps, char *addr);
void proc (int ac, char **av, struct for_flags *f, char *ps);
void output(int fl, struct for_flags *f, char *at, int n, char **av, int ac, int c, int fs);

#endif // SRC_GREP_S21_GREP_H_
