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


class DataRewriter {
   public:
      // The Symtab
      Symtab* symTab;

      // Symtab's region object
      Region* dataRegion;
      Region* bssRegion;

      // A mapping of old offset to new offset
      AddrMapping newLocs;

      // Pointers to the raw bytes
      unsigned char* oldRaw;
      unsigned char* newRaw;

   public:
      DataRewriter(Symtab*);
      VarList filterVariables(Region*);
      void organizeNewData();

};
