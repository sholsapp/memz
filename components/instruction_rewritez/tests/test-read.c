#include <stdio.h>

int var_1 = 100;
int var_2 = 200;
int var_3 = 300;
int var_4 = 400;
int var_5 = 500;
int var_6 = 600;


extern unsigned char *__bss_start;

int main() {

  fprintf(stdout, "%p: %d\n", &var_1, var_1);
  fprintf(stdout, "%p: %d\n", &var_2, var_2);
  fprintf(stdout, "%p: %d\n", &var_3, var_3);
  fprintf(stdout, "%p: %d\n", &var_4, var_4);
  fprintf(stdout, "%p: %d\n", &var_5, var_5);
  fprintf(stdout, "%p: %d\n", &var_6, var_6);

  fprintf(stdout, ".BSS = %p\n", &__bss_start);

  return 0;
}
