#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>

#include "DataRewriter.h"

DataRewriter::DataRewriter(Symtab* _symTab) {
   /**
    *
    */
   symTab = _symTab;

   if (!symTab->findRegion(dataRegion, ".data")) {
      fprintf(stderr, "> Couldn't find .data region!\n");
      exit(EXIT_FAILURE);
   }

   if (!symTab->findRegion(bssRegion, ".bss")) {
      fprintf(stderr, "> Couldn't find .bss region!\n");
      exit(EXIT_FAILURE);
   }

   organizeNewData();

   return;
}


VarList DataRewriter::filterVariables(Region* reg) {
   /**
    *
    */
   VarList all_variables;
   VarList data_variables;
   if (symTab->getAllVariables(all_variables)) {
      for (VarList::iterator it = all_variables.begin(); it != all_variables.end(); ++it) {
         Variable* var = *it;
         // If it is in the appropriate region (string comparison)
         if (var->getRegion() != NULL
               && var->getRegion()->getRegionName().compare(reg->getRegionName()) == 0) {
            // Save a reference to this variable
            data_variables.push_back(var);
         }
      }
   }
   return data_variables;
}


void DataRewriter::organizeNewData() {
   /**
    *
    */
   VarList globals = filterVariables(dataRegion);

   unsigned int dataSize = 0;
   unsigned int dataBase = 0;
   unsigned int dataOffset = 0;

   unsigned int rawIndex = 0;
   unsigned int rawValue = 0;

   unsigned int newRawIndex = 0;
   unsigned int newDataSize = 0;

   dataSize = dataRegion->getRegionSize();
   dataBase = dataRegion->getRegionAddr();
   newDataSize = sizeof(unsigned char) * dataSize * 2;

   oldRaw = (unsigned char*) dataRegion->getPtrToRawData();
   newRaw = (unsigned char*) calloc (1, newDataSize);

   // The dso_handle bullshit
   memcpy(newRaw, oldRaw, 4);
   memset(newRaw + 4, 0xAE, 4);
   newRawIndex += 8;

   //fprintf(stderr, "Region offset = %p\n", (void*) dataBase);
   //for (int i = 0; i < globals.size(); i++) {
   //   dataOffset = globals[i]->getOffset();
   //   rawIndex = dataOffset - dataBase;
   //   // Read a 4 bytes from this offset into an unsigned int
   //   memcpy(&rawValue, oldRaw + rawIndex, 4);
   //   fprintf(stderr, "%p: %s (%d) size (%d)\n",
   //         dataOffset,
   //         globals[i]->getAllPrettyNames()[0].c_str(),
   //         rawValue,
   //         globals[i]->getSize());
   //}

   // Pad the new data section with an extra 4 bytes between all 4 byte values.
   for (int i = 0; i < globals.size(); i++) {
      dataOffset = globals[i]->getOffset();
      rawIndex = dataOffset - dataBase;
      memcpy(newRaw + newRawIndex, oldRaw + rawIndex, 4);
      memset(newRaw + newRawIndex + 4, 0xAE, 4);
      newLocs[dataOffset] = dataBase + newRawIndex;
      newRawIndex += 8;
   }
   //newLocs[0] = 0;

   //unsigned int* tmp = (unsigned int*) newRaw;
   //for (int i = 0; i < dataSize / 2; i++) {
   //   fprintf(stderr, "new raw[%d]: %d\n", i, tmp[i]);
   //}

   //for (AddrMapping::const_iterator iter = newLocs.begin();
   //      iter != newLocs.end();
   //      ++iter) {
   //   fprintf(stderr, "%p -> %p\n", iter->first, iter->second);
   //}

   if (!dataRegion->setPtrToRawData(newRaw, newDataSize)) {
      fprintf(stderr, "Unable to assign new data section.\n");
      exit(EXIT_FAILURE);
   }

}
