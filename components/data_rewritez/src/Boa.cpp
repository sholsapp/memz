#include "Boa.h"

// Comparator used to sort vectors by memory offsets
struct byOffset {
   bool operator()(Variable* const &a, Variable* const &b) {
      return a->getOffset() < b->getOffset();
   }
};

void Boa::debugDescribeRegion(Region* reg) {
   // Dump some debug information
   fprintf(stderr, "getRegionAddr(): %p\n", (void*) reg->getRegionAddr());
   fprintf(stderr, "getRegionSize(): %ld\n", reg->getRegionSize());
   fprintf(stderr, "getDiskOffset(): %p\n", (void*) reg->getDiskOffset());
   fprintf(stderr, "getDiskSize(): %ld\n", reg->getDiskSize());
   fprintf(stderr, "getPtrToRawData(): %p\n", (void*) reg->getPtrToRawData());
   return;
}

/*
 * Constructor creates the Symtab object and replaces the data region (though, it doesn't change anything about the data region, quite yet).
 */
Boa::Boa(std::string _binary_name) {

   // Save the name of the binary
   binary_name = _binary_name;

   // Try opening and parsing the symbol table
   if (!Symtab::openFile(symtab_obj, binary_name)) {
      // Print message to screen
      fprintf(stderr, "> Couldn't open binary!\n");
      exit(EXIT_FAILURE);
   }

   // Try getting a handle on the DEFAULT_MODULE
   if (!symtab_obj->findModuleByName(module, "DEFAULT_MODULE")) {
      fprintf(stderr, "> Couldn't find DEFAULT_MODULE\n");
      exit(EXIT_FAILURE);
   }

   /*
   // Get a list of all the data regions and print each of their symbol names
   RegionList regions;
   if (symtab_obj->getDataRegions(regions)) {
   for (int i = 0; i < regions.size(); ++i) {
   fprintf(stderr, "Region name = %s\n", regions[i]->getRegionName().c_str());
   }
   }
   */

   // Try to find the '.data' region, and save a reference to it
   Region* data = NULL;
   if (symtab_obj->findRegion(data, ".data")) {

      // Save a reference for later
      data_region = data;

      // Dump some debug information
      debugDescribeRegion(data_region);


      // This is the current/original data region
      int* raw = (int*) data_region->getPtrToRawData();

      // This will be the new data region
      int* new_raw = (int*) malloc (sizeof(char) * data_region->getRegionSize());

      // Copy the current/original data into the new buffer, print out the data as we go
      for (int i = 0; i < data_region->getRegionSize() / 4; ++i) {
         fprintf(stderr, "Data[%d] = %d\n", i, raw[i]);

         // Proof of concept test
         if (raw[i] == 100) {
            fprintf(stderr, "Changing data at offset(%p)!\n", (void*) (raw + i));
            new_raw[i] = 500;
         }
         else {
            new_raw[i] = raw[i];
         }
      }

      // Assign the data region to point at the new buffer
      if (!data_region->setPtrToRawData((void*) new_raw, data_region->getRegionSize())) {
         fprintf(stderr, "Failed to set pointer to raw data!\n");
      }

      // Dump the same debug information
      debugDescribeRegion(data_region);
   }

   // Emit the binary
   if (symtab_obj->emit("test.out")) {
      fprintf(stderr, "Emit success\n");
   }
   else {
      fprintf(stderr, "Emit failure\n");
   }

   debugDumpVariables();

   // Emit the binary
   if (symtab_obj->emit("test.out")) {
      fprintf(stderr, "Emit success\n");
   }
   else {
      fprintf(stderr, "Emit failure\n");

   }

   return;
}

string Boa::getBinaryName() {
   return binary_name;
}

void Boa::debugDumpVariables() {
   VarList variables;
   // Try to get all global and static variables
   if (symtab_obj->getAllVariables(variables)) {

      // Sort the vector based on the offsets of each variable
      std::sort(variables.begin(), variables.end(), byOffset());

      for(VarList::iterator it = variables.begin(); it != variables.end(); ++it) {
         Variable* var = *it;

         fprintf(stderr, "size(%d) ", var->getSize());
         // For each variable, print out all of its "pretty names"
         StrList names = var->getAllPrettyNames();
         for(StrList::iterator strit = names.begin(); strit != names.end(); ++strit) {
            fprintf(stderr, "offset(%p) name(%s) region(%s)", (void*) var->getOffset(), strit->c_str(), var->getRegion()->getRegionName().c_str());

            if (strit->compare("global_var_1") == 0) {
               fprintf(stderr, "Found global_var_1!!\n");
               if (symtab_obj->deleteVariable(var)) {
                  fprintf(stderr, "Successfully deleted global_var_1\n");
               }
            }
         }
         fprintf(stderr, "\n");
      }
   }
   else {
      fprintf(stderr, "Could not find variables\n");
   }
   return;
}

void Boa::debugDumpFunctions() {
   FuncList functions;
   if (symtab_obj->getAllFunctions(functions)) {
      for (FuncList::iterator it = functions.begin(); it != functions.end(); ++it) {
         Function* func = *it;
         StrList names = func->getAllPrettyNames();
         for(StrList::iterator strit = names.begin(); strit != names.end(); ++strit) {
            fprintf(stderr, "Function name = %s\n", strit->c_str());
            LocalVarList lvars;
            if (func->getLocalVariables(lvars)) {
               for (int j = 0; j < lvars.size(); ++j) {
                  fprintf(stderr, "\tLocal var = %s\n", lvars[j]->getName().c_str());
               }
            }
            else {
               fprintf(stderr, "Could not fetch local variables for %s\n", strit->c_str());
            }
         }
         fprintf(stderr, "\n");
      }
   }
   else {
      fprintf(stderr, "Could not find functions\n");
   }
   return;
}
