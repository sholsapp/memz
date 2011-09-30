#ifndef _BINARYREWRITER_H_
#define _BINARYREWRITER_H_

#include <Symbol.h>
#include <Module.h>
#include <Variable.h>
#include <InstructionDecoder.h>
#include <Immediate.h>
//#include <Function.h>
//#include <Aggregate.h>
//#include <dyntypes.h>

using namespace std;

using namespace Dyninst;
using namespace SymtabAPI;
using namespace InstructionAPI;


struct DataRegionEntryInfo {
   string prettyName;
   unsigned int size;
   unsigned int value;
};


typedef vector<Variable*> VarList;
//typedef vector<localVar*> LocalVarList;
//typedef vector<Function*> FuncList;
typedef vector<Region*> RegionList;
typedef vector<string> StrList;

// Comparator used to sort vectors by memory offsets
struct byOffset {
   bool operator()(Variable* const &a, Variable* const &b) {
      return a->getOffset() < b->getOffset();
   }
};

class InstructionRewriter {
   private:
      // The main symtab object
      Symtab* symtab_obj;
      Module* module;
      Region* data_region;
      Region* text_region;
      string binary_name;

   private:
      VarList collectGlobalVariables(Region* reg);
      map<unsigned int, struct DataRegionEntryInfo> describeData();

   public:
      InstructionRewriter(string binary_name);

      void describeRegion(Region* reg);
};

#endif
