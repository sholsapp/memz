#include <string>

#include <Symbol.h>
#include <Module.h>
#include <Variable.h>
#include <InstructionDecoder.h>
#include <Immediate.h>

#include "DataRewriter.h"
#include "TextRewriter.h"

using namespace std;

using namespace Dyninst;
using namespace SymtabAPI;

class BinaryRewriter {
   public:
      Symtab* symTab;
      Module* module;
      string binaryName;
   public:
      BinaryRewriter(string);
      void describeRegion(Region*);

};
