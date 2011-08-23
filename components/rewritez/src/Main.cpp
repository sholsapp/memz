#include <cstdio>

#include "Boa.h"

// Custom comparator to sort BPatch_variableExpr by variable base address
struct byBaseAddr {
  bool operator()(BPatch_variableExpr* const &a, BPatch_variableExpr* const &b) {
    return a->getBaseAddr() < b->getBaseAddr();
  }
};


int main(int argc, char** argv) {

  if (argc < 2) {
    fprintf(stdout, "Usage: ./program <binary to rewrite>\n");
    exit(EXIT_SUCCESS);
  }

  // The constructor does all the work currently
  Boa boa(argv[1]);

  fprintf(stderr, "Working on %s\n", boa.getBinaryName().c_str());

  //boa.debugDumpVariables();

  return 0;
}



