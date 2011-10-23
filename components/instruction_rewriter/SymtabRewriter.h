#include <Symbol.h>
#include <Module.h>
#include <Variable.h>
#include <InstructionDecoder.h>
#include <Immediate.h>

#include <string>

#include "Types.h"

using namespace std;

using namespace Dyninst;
using namespace SymtabAPI;


class SymtabRewriter {
   public:
      // The Symtab
      Symtab* symTab;

   public:
      SymtabRewriter(Symtab*, AddrMapping*);
};
