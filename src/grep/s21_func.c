#include "s21_grep.h"
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <regex.h>
#include <sys/types.h>
#include <string.h>
FILE *mystream;

int options_flags(int ac, char **av, struct for_flags *f, char *ps) {
  int opt;
  int err = 0;
  while ((opt = getopt_long(ac, av, "e:ivclnhsf:o", NULL, NULL)) != -1) {
  //   for (long unsigned int n = 0; n < (strlen(*av)); n++) {
  //   printf("%s ", av[n]);
  // }
  // printf("\n");
  // printf("%s\n\n\n", ps);
    switch (opt) {
    case 'e':
      f->f_e = 1;
      formOptArg(ps, optarg);
      break;
    case 'i':
      f->f_i = 1;
      break;
    case 'v':
      f->f_v = 1;
      break;
    case 'c':
      f->f_c = 1;
      break;
    case 'l':
      f->f_l = 1;
      break;
    case 'n':
      f->f_n = 1;
      break;
    case 'h':
      f->f_h = 1;
      break;
    case 's':
      f->f_s = 1;
      break;
    case 'f':
      f->f_f = 1;
      if (openFile(optarg) == 1) {
        if (f->f_s == 0)
        output(7, 0, 0, 0, av, 0, 0, 0);
      } else {
        char pattemp[1024];
        while ((getString(pattemp)) != 1) {
          formOptArg(ps, pattemp);
        }
      }
      fclose(mystream);
      break;
    case 'o':
      f->f_o = 1;
      break;
    default:
      err = 1;
    }
      // printf("%d\n", (ac - optind));
  }
  // for (long unsigned int n = 0; n < (strlen(*av)); n++) {
  //   printf("%s ", av[n]);
  // }
  // printf("\n\n");
  //  printf("%d\t%s\tПошОл На ХУЙ!!!\t%s\n\n\n", ac, av[optind], ps);
  if ((ac - optind) <= 2 || (av[(optind + 1)] == NULL))
    f->f_h = 1;
  // printf("%d\n", f->f_h);  
  return err;
}

int openFile(char *filename) {
  int fl = 0;
  if ((mystream = fopen(filename, "r")) == NULL) {
    fl = 1;
  }
  return fl;
}

int getString(char *strarr) {
  int fl = 0;
    if (fgets(strarr, 8192, mystream) == NULL) {
    fl = 1;
  } else {
    if ((strarr[strlen(strarr) - 1]) == 10)
      strarr[strlen(strarr) - 1] = 0;
  }
  return fl;
}

void formOptArg(char *ps, char *addr) {
  if (strlen(ps) == 0) {
    strcat(ps, addr);
  } else {
    strcat(ps, "|");
    strcat(ps, addr);
  }
}

void proc (int ac, char **av, struct for_flags *f, char *ps) {
  regex_t myregex = {0};
  int schet = 0;  // счетчик нумератора строк
  int count = 0;  // счетчик количества совпавших строк
  int ucount = 0;  // счетчик количества несовпавших строк
  int fs = 2;  // флаг совпадения: 1 есть 2 нет
  char targstr[8192];  // объявление массива для исходной строки из потока
  char msgbuf[128];  //  объявление массива для ошибки
  int compadd;
    if ((f->f_e == 0) && (f->f_f == 0)) {
    formOptArg(ps, av[optind]);
    optind++;
    }
    // printf("%d\t%s\tПошОл На ХУЙ!!!\t%s\n\n\n", ac, av[optind], ps);
  if (f->f_i == 0) {  //  flag_i
    compadd = regcomp(&myregex, ps, REG_EXTENDED);
  } else {
    compadd = regcomp(&myregex, ps, REG_EXTENDED|REG_ICASE);
  }
  if (compadd != 0) {
    output(5, 0, 0, 0, 0, 0, 0, 0);
  } else {
    for (int cfiles = optind; cfiles < ac; cfiles++) {  //  цикл перебора файлов, где искать
      if (openFile(av[cfiles]) == 1) {  // открытие потока из файла
        if (f->f_s == 0)  //  flag_s
        output(2, 0, 0, 0, av, 0, cfiles, 0);
      } else {
        while (getString(targstr) == 0) {
          schet++;
          if ((compadd = regexec(&myregex, targstr, 0, NULL, 0)) > 1) {  // если ошибка сравнения
          regerror(compadd, &myregex, msgbuf, sizeof(msgbuf));
          output(6, 0, msgbuf, 0, 0, 0, 0, 0);
          } else if (compadd == 1) {  // если несовпадение
          fs = 1;
// printf("%d\n", fs);
          ucount++;

          } else {
          fs = 0;
// printf("%d\n", fs);
          count++;
          }
          if (f->f_c == 0 && f->f_l == 0)
            output(0, f, targstr, schet, av, ac, cfiles, fs);
        }   //  while взятие строк из потока
        if (f->f_c == 1 || f->f_l == 1) {
          if (f->f_v == 0) {
            output(1, f, targstr, count, av, ac, cfiles, fs);
          } else {
            output(1, f, targstr, ucount, av, ac, cfiles, fs);
          }
        }
      schet = 0;
      count = 0;
      ucount = 0;
      fs = 2;
      fclose(mystream);
      }   //  если файл существует и корректно открылся
    }   //  for
  }   //  если регулярное выражение скомпилировано корректно
  regfree(&myregex);
}

void output(int fl, struct for_flags *f, char *at, int n, char **av, int ac, int c, int fs) {
  switch (fl) {
    case 0:
// printf("%d\t%d\t%d\n", f->f_v, f->f_h, fs);
      if (((f->f_v == 0) && (fs == 0)) || ((f->f_v == 1) && (fs == 1))) {
        if (f->f_h == 0)
          printf("%s:", av[c]);    //  имя файла
        if (f->f_n == 1)
          printf("%d:", n);    //  номер строки в файле
        printf("%s%c", at, 10);
      }
      break;
    
    case 1:
      if (f->f_l == 1 && f->f_c == 1) {
        if (f->f_h == 0)
          printf("%s:", av[c]);
        printf("1\n%s\n", av[c]);
      } else if (f->f_c == 1 && f->f_l == 0) {
        if (f->f_h == 0)
          printf("%s:", av[c]);
        printf("%d\n", n);
      } else
        printf("%s\n", av[c]);
      break;
      // if (f->f_c == 1) {
      //   printf("%d", c);    //  количество совпадающих строк



    // case 0:
    //   

    // case 1:
    //   if (ac > (optind + 2) && f->f_h == 0)
    //     printf("%s:", av[c]);
    //   if (f->f_o == 1 && f->f_v == 0)
    //     at = av[optind];
    //   if (f->f_n == 1)
    //     printf("%d:", n);
    //   printf("%s%c", at, 10);
    //   break;

    case 2:
      printf("%s: %s: No such file or directory\n", av[0], av[c]);
      break;
    case 3:
      fprintf(stderr, "Too few arguments\n");
     break;
    case 4:
      fprintf(stderr, "usage: %s [-eivclnhsfo] [file ...]\n", av[0]);
     break;
    case 5:
      fprintf(stderr, "Could not compile regex\n");
     break;
    case 6:
      fprintf(stderr, "Regex match failed: %s\n", at);
      break;
    case 7:
      fprintf(stderr, "%s: %s: No such file or directory\n", av[0], optarg);
     break;
    default:
      fprintf(stderr, "Ни ХУЯ у тебя ничего не вышло!!!\n\t%d\t%d\n", ac, fs);
  }
}
