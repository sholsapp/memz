#ifndef BOA_H_
#define BOA_H_

// System
#include <string>
#include <iostream>

// Dyninst
#include <BPatch.h>

#include <dyntypes.h>

//Symtab
#include <Module.h>
#include <Variable.h>
#include <Function.h>
#include <Aggregate.h>

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

// Wrapper over Dyninst functionality
class Boa {

  private:
    string binary_name;

    Symtab* symtab_obj;

    Module* module;
 
    Region* data_region;

    void debugDescribeRegion(Region* reg);

    //void addGlobalVariable();
    //void deleteGlobalVariable();

  public:
    /*
     * Constructor takes the path of the binary to modify
     */
    Boa(string binary_name);

    /*
     * Accessor for binary name
     */
    string getBinaryName();

    //bool addGlobalVariable();

    // Function to get base pointer to data region
    // Function to add new variables
    // Function to delete old variable

    /*
     * Dumps information 
     */
    void debugDumpVariables();
    void debugDumpFunctions();
     
};

#endif
