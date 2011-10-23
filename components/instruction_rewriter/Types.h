#include <string>
#include <cstdio>
#include <algorithm>
#include <vector>

#include <Symbol.h>
#include <Module.h>
#include <Variable.h>
#include <InstructionDecoder.h>
#include <Immediate.h>

typedef std::vector<Dyninst::SymtabAPI::Variable*> VarList;
typedef std::vector<Dyninst::SymtabAPI::Symbol*> SymList;

typedef std::map<unsigned int, unsigned int> AddrMapping;


