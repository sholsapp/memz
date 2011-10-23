#include "Types.h"

using namespace std;

using namespace Dyninst;
using namespace SymtabAPI;
using namespace InstructionAPI;

class TextRewriter {
  public:
     Symtab* symTab;
     Region* dataRegion;
     Region* bssRegion;
     Region* textRegion;

     AddrMapping* relocs;

     unsigned char* oldText;
     unsigned char* newText;

   public:
     TextRewriter(Symtab*, AddrMapping*);
     void organizeNewText();
};


