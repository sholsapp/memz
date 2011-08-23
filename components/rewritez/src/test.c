#include <stdio.h>

int global_var_1 = 100;
int gloval_var_2 = 200;

int main() {

  fprintf(stdout, "%d\n", global_var_1);

  return global_var_1;
}
