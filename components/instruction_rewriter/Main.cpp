#include <cstdio>

#include "BinaryRewriter.h"

int main(int argc, char** argv) {
   /**
    *
    */
   if (argc < 2) {
      fprintf(stdout, "Usage: ./program <binary to rewrite>\n");
      exit(EXIT_SUCCESS);
   }

   BinaryRewriter rewriter(argv[1]);

   return 0;
}
