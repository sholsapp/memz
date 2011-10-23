#include "BinaryRewriter.h"

BinaryRewriter::BinaryRewriter(std::string whichBinary) {
   /**
    *
    */
   binaryName = whichBinary;

   if (!Symtab::openFile(symTab, whichBinary)) {
      fprintf(stderr, "> Couldn't open binary!\n");
      exit(EXIT_FAILURE);
   }

   // Try getting a handle on the DEFAULT_MODULE
   if (!symTab->findModuleByName(module, "DEFAULT_MODULE")) {
      fprintf(stderr, "> Couldn't find DEFAULT_MODULE\n");
      exit(EXIT_FAILURE);
   }

   DataRewriter dataRewriter(symTab);

   describeRegion(dataRewriter.dataRegion);

   describeRegion(dataRewriter.bssRegion);

   TextRewriter textRewriter(symTab, &dataRewriter.newLocs);

   // Emit the binary
   if (symTab->emit(binaryName + "-mutated")) {
      fprintf(stderr, "Emit success\n");
   } else {
      fprintf(stderr, "Emit failure\n");
   }


}


void BinaryRewriter::describeRegion(Region* reg) {
   /**
    *
    */
   if (reg != NULL) {
      fprintf(stderr, "Region (%s):\n", reg->getRegionName().c_str());
      fprintf(stderr, "\tgetRegionAddr(): %p\n", (void*) reg->getRegionAddr());
      fprintf(stderr, "\tgetRegionSize(): %ld\n", reg->getRegionSize());
      fprintf(stderr, "\tgetDiskOffset(): %p\n", (void*) reg->getDiskOffset());
      fprintf(stderr, "\tgetDiskSize(): %ld\n", reg->getDiskSize());
      fprintf(stderr, "\tgetPtrToRawData(): %p\n", (void*) reg->getPtrToRawData());
      fprintf(stderr, "\n");
   }
   else {
      fprintf(stderr, "Region is NULL\n");
   }
}
