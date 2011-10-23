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
   assert(symtab_obj != NULL);

   //std::vector<Module*> modules;
   //if (symtab_obj->getAllModules(modules)) {
   //  fprintf(stderr, "There are %d many modules\n", modules.size());
   //  fprintf(stderr, "Module name = %s\n", modules[0]->fullName().c_str());
   //}

   // Try getting a handle on the DEFAULT_MODULE
   if (!symtab_obj->findModuleByName(module, "DEFAULT_MODULE")) {
   //if (!symtab_obj->findModuleByName(module, "libtest.so")) {
      fprintf(stderr, "> Couldn't find DEFAULT_MODULE\n");
      exit(EXIT_FAILURE);
   }
   assert(module != NULL);

   // Try to find the '.data' region
   if (!symtab_obj->findRegion(data_region, ".data")) {
      fprintf(stderr, "> Couldn't find .data region!\n");
      exit(EXIT_FAILURE);
   }
   assert(data_region != NULL);

   // Dump some debug information
   debugDescribeRegion(data_region);

   // This is the current/original data region
   char* raw = (char*) data_region->getPtrToRawData();

   // We make a copy of it
   orig_raw = (char*) malloc(sizeof(char) * data_region->getRegionSize());
   memcpy(orig_raw, raw, data_region->getRegionSize());

   // This will be the new data region
   new_raw = (char*) malloc (sizeof(char) * data_region->getRegionSize());
   memcpy(new_raw, orig_raw, data_region->getRegionSize());

   // Collect all the variables in the .data section
   collectGlobalVariables(data_region);

   /**
    *
    * DEBUG
    *
    */


   fprintf(stderr, "Region offset = %p\n", (void*) data_region->getRegionAddr());

   for (int i = 0; i < 6; ++i) {
      void* addr = (void*) orig_data_variables[i]->getOffset();
      unsigned int size = orig_data_variables[i]->getSize();
      void* guess = (char*) addr + size;

      fprintf(stderr, "offset (%p) size (%d) next var guess (%p)\n", addr, size, guess);
   }

   symtab_obj->deleteVariable(orig_data_variables[0]);
   symtab_obj->deleteVariable(orig_data_variables[1]);
   symtab_obj->deleteVariable(orig_data_variables[2]);
   symtab_obj->deleteVariable(orig_data_variables[3]);
   symtab_obj->deleteVariable(orig_data_variables[4]);
   symtab_obj->deleteVariable(orig_data_variables[5]);


   symtab_obj->addSymbol(new Symbol("var_6", Symbol::ST_OBJECT, Symbol::SL_GLOBAL, Symbol::SV_DEFAULT, orig_data_variables[0]->getOffset(), module, data_region, sizeof(int), false, true));
   symtab_obj->addSymbol(new Symbol("var_5", Symbol::ST_OBJECT, Symbol::SL_GLOBAL, Symbol::SV_DEFAULT, orig_data_variables[1]->getOffset(), module, data_region, sizeof(int), false, true));
   symtab_obj->addSymbol(new Symbol("var_4", Symbol::ST_OBJECT, Symbol::SL_GLOBAL, Symbol::SV_DEFAULT, orig_data_variables[2]->getOffset(), module, data_region, sizeof(int), false, true));
   symtab_obj->addSymbol(new Symbol("var_3", Symbol::ST_OBJECT, Symbol::SL_GLOBAL, Symbol::SV_DEFAULT, orig_data_variables[3]->getOffset(), module, data_region, sizeof(int), false, true));
   symtab_obj->addSymbol(new Symbol("var_2", Symbol::ST_OBJECT, Symbol::SL_GLOBAL, Symbol::SV_DEFAULT, orig_data_variables[4]->getOffset(), module, data_region, sizeof(int), false, true));
   symtab_obj->addSymbol(new Symbol("var_1", Symbol::ST_OBJECT, Symbol::SL_GLOBAL, Symbol::SV_DEFAULT, orig_data_variables[5]->getOffset(), module, data_region, sizeof(int), false, true));

   //fprintf(stderr, "What module = %s\n", temp->getRegion()->getRegionName().c_str());

   /**
    *
    * END DEBUG
    *
    */

   // Assign the data region to point at the new buffer
   if (!data_region->setPtrToRawData((void*) new_raw, data_region->getRegionSize())) {
      fprintf(stderr, "Failed to set pointer to raw data!\n");
      exit(EXIT_FAILURE);
   }

   // Emit the binary
   if (symtab_obj->emit(binary_name + "-mutated")) {
      fprintf(stderr, "Emit success\n");
   }
   else {
      fprintf(stderr, "Emit failure\n");
   }

   return;
}

Boa::~Boa() {
   fprintf(stderr, "Destructor\n");
   free(orig_raw);
}

int Boa::collectGlobalVariables(Region* inRegion) {
   VarList all_variables;
   int variables_collected = 0;
   // Try to get all the variables in all sections
   if (symtab_obj->getAllVariables(all_variables)) {
      // For each variable
      for (VarList::iterator it = all_variables.begin(); it != all_variables.end(); ++it) {
         Variable* var = *it;
         // If it is in the appropriate region (string comparison)
         if (var->getRegion() != NULL && var->getRegion()->getRegionName().compare(inRegion->getRegionName()) == 0) {
            // Save a reference to this variable
            orig_data_variables.push_back(var);
            // Up the counter
            variables_collected++;
         }
      }
   }
   else {
      fprintf(stderr, "Failed to get all variables!\n");
      exit(EXIT_FAILURE);
   }
   return variables_collected;
}

void Boa::debugDumpVariables() {
   VarList variables;
   // Try to get all global and static variables
   if (symtab_obj->getAllVariables(variables)) {

      // Sort the vector based on the offsets of each variable
      sort(variables.begin(), variables.end(), byOffset());

      for(VarList::iterator it = variables.begin(); it != variables.end(); ++it) {
         Variable* var = *it;

         fprintf(stderr, "size(%d) ", var->getSize());
         // For each variable, print out all of its "pretty names"
         StrList names = var->getAllPrettyNames();
         for(StrList::iterator strit = names.begin(); strit != names.end(); ++strit) {
            fprintf(stderr, "offset(%p) name(%s) region(%s)", (void*) var->getOffset(), strit->c_str(), var->getRegion()->getRegionName().c_str());

            //if (strit->compare("global_var_1") == 0) {
            //   fprintf(stderr, "Found global_var_1!!\n");
            //   if (symtab_obj->deleteVariable(var)) {
            //      fprintf(stderr, "Successfully deleted global_var_1\n");
            //      fprintf(stderr, "var offset after deletion = %p\n", (void*) var->getOffset());
            //   }
            //}
         }
         fprintf(stderr, "\n");
      }
   }
   else {
      fprintf(stderr, "Could not find variables\n");
   }
   return;
}
