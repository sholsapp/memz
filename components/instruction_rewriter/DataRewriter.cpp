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

   organizeNewData(dataRegion, 0xAA);
   organizeNewBss(bssRegion, 0xBB);

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


void DataRewriter::organizeNewData(Region* reg, unsigned char magic) {
   /**
    *
    */
   VarList globals = filterVariables(reg);

   unsigned int dataSize = 0;
   unsigned int dataBase = 0;
   unsigned int dataOffset = 0;

   unsigned int rawIndex = 0;
   unsigned int rawValue = 0;

   unsigned int newRawIndex = 0;

   dataSize = reg->getRegionSize();
   dataBase = reg->getRegionAddr();

   newDataBase = dataBase;
   newDataSize = sizeof(unsigned char) * dataSize * 2;

   oldRaw = (unsigned char*) reg->getPtrToRawData();
   newRaw = (unsigned char*) malloc (newDataSize);

   // The dso_handle bullshit
   memcpy(newRaw, oldRaw, 4);
   memset(newRaw + 4, magic, 4);
   newRawIndex += 8;

   // Pad the new data section with an extra 4 bytes between all 4 byte values.
   for (int i = 0; i < globals.size(); i++) {
      dataOffset = globals[i]->getOffset();
      rawIndex = dataOffset - dataBase;
      memcpy(newRaw + newRawIndex, oldRaw + rawIndex, 4);
      // Sanity checks, write magic values into padding
      memset(newRaw + newRawIndex + 4, magic, 4);
      // Update the relocated address mapping
      newLocs[dataOffset] = dataBase + newRawIndex;
      newRawIndex += 8;
   }

   fprintf(stderr, "NEW DATA: %p through %p\n", newDataBase, newDataBase + newDataSize);

   if (!reg->setPtrToRawData(newRaw, newDataSize)) {
      fprintf(stderr, "Unable to assign new data section.\n");
      exit(EXIT_FAILURE);
   }

}


void DataRewriter::organizeNewBss(Region* reg, unsigned char magic) {
   /**
    *
    */

   fprintf(stderr, "BSS region starts at %p\n", reg->getRegionAddr());

   VarList globals = filterVariables(reg);

   unsigned int dataSize = 0;
   unsigned int dataBase = 0;
   unsigned int dataOffset = 0;

   unsigned int rawIndex = 0;
   unsigned int rawValue = 0;

   unsigned int newRawIndex = 0;

   dataSize = reg->getRegionSize();
   dataBase = newDataBase + newDataSize;

   fprintf(stderr, "BSS base = %p\n", (void*) dataBase);

   unsigned int newBssSize = sizeof(unsigned char) * dataSize;

   oldRawBss = (unsigned char*) reg->getPtrToRawData();
   newRawBss = (unsigned char*) malloc (newBssSize);

   memcpy(newRawBss, oldRawBss, dataSize);


   for (int i = 0; i < globals.size(); i++) {
      dataOffset = globals[i]->getOffset();
      rawIndex = dataOffset - dataBase;
      // Update the relocated address mapping
      newLocs[dataOffset] = dataBase + newRawIndex;
      newRawIndex += globals[i]->getSize();
   }

   for (AddrMapping::const_iterator iter = newLocs.begin();
         iter != newLocs.end();
         ++iter) {
      fprintf(stderr, "%p -> %p\n", iter->first, iter->second);
   }

   // Causes a segfault in the mutated binary
   // 
   //if (!reg->setPtrToRawData(newRawBss, newDataSize)) {
   //   fprintf(stderr, "Unable to assign new data section.\n");
   //   exit(EXIT_FAILURE);
   //}

}

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

//unsigned int* tmp = (unsigned int*) newRaw;
//for (int i = 0; i < dataSize / 2; i++) {
//   fprintf(stderr, "new raw[%d]: %d\n", i, tmp[i]);
//}

//for (AddrMapping::const_iterator iter = newLocs.begin();
//      iter != newLocs.end();
//      ++iter) {
//   fprintf(stderr, "%p -> %p\n", iter->first, iter->second);
//}

