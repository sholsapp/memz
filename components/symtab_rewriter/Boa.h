#ifndef BOA_H_
#define BOA_H_

// System
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cassert>

//Symtab
#include <Symbol.h>
#include <Module.h>
#include <Variable.h>
#include <Function.h>
#include <Aggregate.h>
#include <dyntypes.h>

// Namespace Qualification
using namespace std;
using namespace Dyninst;
using namespace SymtabAPI;

// Custom typedefs
typedef vector<Variable*> VarList;
typedef vector<localVar*> LocalVarList;
typedef vector<Function*> FuncList;
typedef vector<Region*> RegionList;
typedef vector<string> StrList;

/**
 * Buffer Overflow Attack (Boa) class attempts to prevent buffer overflow attacks by
 *  reordering and padding the '.data' section of binaries.
 */
class Boa {

   private: // Data

      // The main symtab object
      Symtab* symtab_obj;

      // A handle to the DEFAULT_MODULE
      Module* module;

      // The '.data' region for the DEFAULT_MODULE
      Region* data_region;

      // A list of all the original global variables
      VarList orig_data_variables;

      // A buffer of the original data
      char* orig_raw;

      // A buffer of the new data
      char* new_raw;

      // Name of the binary we're working on
      string binary_name;

   private: // Methods
      /**
       * Collects references to all of the SymtabAPI::Variable objects in the original symbol table.
       */
      int collectGlobalVariables(Region* inRegion);

      /**
       * Describes provided SymtabAPI::Region object.
       */
      void debugDescribeRegion(Region* reg);

   public:
      /**
       * Constructor takes the path of the binary to modify
       */
      Boa(string binary_name);

      /**
       * Destructor
       */
      ~Boa();

      /**
       * Accessor for binary name
       */
      string getBinaryName() {return binary_name;}

      /**
       * Dumps information
       */
      void debugDumpVariables();

};

#endif
