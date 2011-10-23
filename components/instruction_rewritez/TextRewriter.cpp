#include "TextRewriter.h"

TextRewriter::TextRewriter(Symtab* _symTab, AddrMapping* relocations) {
   /**
    *
    */
   symTab = _symTab;

   relocs = relocations;

   if (!symTab->findRegion(dataRegion, ".data")) {
      fprintf(stderr, "> Couldn't find .data region (2)!\n");
      exit(EXIT_FAILURE);
   }

   if (!symTab->findRegion(textRegion, ".text")) {
      fprintf(stderr, "> Couldn't find .text region!\n");
      exit(EXIT_FAILURE);
   }

   if (!symTab->findRegion(bssRegion, ".bss")) {
      fprintf(stderr, "> Couldn't find .bss region!\n");
      exit(EXIT_FAILURE);
   }


   organizeNewText();

   return;
}

void TextRewriter::organizeNewText() {
   /**
    *
    */

   unsigned int textSize = 0;

   textSize = textRegion->getRegionSize();

   oldText = (unsigned char*) textRegion->getPtrToRawData();
   newText = (unsigned char*) calloc (1, sizeof(unsigned char) * textSize);
   memcpy(newText, oldText, textSize);

   //InstructionDecoder decoder(oldText, textSize, Arch_x86_64);
   InstructionDecoder decoder(oldText, textSize, Arch_x86);
   Instruction::Ptr i = decoder.decode();

   long unsigned int currentTextOffset = 0;
   while (i != NULL) {

      //vector<Operand> operands;
      //i->getOperands(operands);
      //if (operands.size() > 0) {
      //  Expression::Ptr exp = operands[0].getValue();
      //  Result res = exp->eval();
      //  Immediate::makeImmediate(Result(u64, 2^32));
      //  fprintf(stderr, "results: %s\n", res.format().c_str());
      //}

      unsigned char* dotTextRaw = (unsigned char*) i->ptr();

      //fprintf(stderr, "%i bytes > %s -- ", i->size(), i->format().c_str());
      //for (int x = 0; x < i->size(); x++) {
      //   fprintf(stderr, " %x ", dotTextRaw[x]);
      //}
      //fprintf(stderr, "\n");

      if (i->readsMemory()) {

         if (/*dotTextRaw[0] == 0xa1 && */i->size() == 5 || i->size() == 6) {
            unsigned int* dataOperand = i->size() == 5 ? (unsigned int*)(dotTextRaw + 1)
               : (unsigned int*)(dotTextRaw + 2);

            // Interpret as int to reverse bytes in memory automatically
            fprintf(stderr, "Data operand: %p\n", (void*) ((unsigned int)*dataOperand));


            unsigned int data = *dataOperand;

            unsigned char* tmp = (unsigned char*) dataOperand;

            if (dataRegion->isOffsetInRegion((*relocs)[data])
                  || bssRegion->isOffsetInRegion((*relocs)[data])) {
               // Hacky, depends on teh 32-bit instructions
               int tmp = i->size() == 5 ? 1 : 2;
               *((unsigned int*)(newText + currentTextOffset + tmp)) = (*relocs)[data];
            }
         }
      }

      //if (dotTextRaw[0] == 0xa1 && i->size() == 9) {
      //   fprintf(stdout, "%i bytes > %s\n", i->size(), i->format().c_str());
      //   unsigned int* dataOperand = (unsigned int*)(dotTextRaw + 1);
      //   if (dataOperand[0] > 134518284) {
      //      ((unsigned int*)(((unsigned char*) dotTextRawMuta) + currentTextOffset + 1))[0] = 134518520;
      //   }
      //}

      currentTextOffset += i->size();
      i = decoder.decode();
   }

   // Assign the data region to point at the new buffer
   if (!textRegion->setPtrToRawData((void*) newText, textRegion->getRegionSize())) {
      fprintf(stderr, "Failed to set pointer to raw text!\n");
      exit(EXIT_FAILURE);
   }
}
