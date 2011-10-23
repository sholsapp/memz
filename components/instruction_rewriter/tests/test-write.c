#include <stdio.h>

int var_1 = 100;
int var_2 = 200;
int var_3 = 300;

int main() {

  fprintf(stdout, "%d\n", var_1);
  fprintf(stdout, "%d\n", var_2);
  fprintf(stdout, "%d\n", var_3);
  var_1 = 400;
  var_2 = 500;
  var_3 = 600;
  fprintf(stdout, "%d\n", var_1);
  fprintf(stdout, "%d\n", var_2);
  fprintf(stdout, "%d\n", var_3);

  return 0;
}
