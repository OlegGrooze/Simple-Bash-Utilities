#include <stdio.h>
#ifndef SRC_CAT_FUNC_H_
#define SRC_CAT_FUNC_H_

struct for_flags {
  int f_b, f_E, f_n, f_s, f_T, f_e, f_t, f_v;
};
int opt_flags(int c, char *argv[], struct for_flags *f);
void proc(struct for_flags *f, FILE *ms);
void output(int f, int n, int symb, int c, char *argv[]);

#endif // SRC_CAT_FUNC_H_
