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

   InstructionDecoder decoder(oldText, textSize, Arch_x86_64);
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
      //fprintf(stdout, "%i bytes > %s\n", i->size(), i->format().c_str());

      unsigned char* dotTextRaw = (unsigned char*) i->ptr();

      if (i->readsMemory()) {
         // MOV instruction loading a 32 bit val
         if (dotTextRaw[0] == 0xa1 && i->size() == 9) {

            unsigned int* dataOperand = (unsigned int*)(dotTextRaw + 1);

            unsigned int data = *dataOperand;

            //fprintf(stderr, "Looking for %p, found %p\n", (void*)data, (void*) (*relocs)[data]);
            if (dataRegion->isOffsetInRegion((*relocs)[data])) {
               *((unsigned int*)(newText + currentTextOffset + 1)) = (*relocs)[data];

               //fprintf(stderr, "Rewrote .text:\n");
               //fprintf(stderr, "%d byte : < %s >\n",
               //      i->size(),
               //      i->format().c_str());
               //fprintf(stderr, "New data address at : %d\n", (*relocs)[data]);
            }
            else {
               fprintf(stderr, "NO on %p\n", (void*) data);
            }

            //vector<Operand> operands;
            //i->getOperands(operands);
            //fprintf(stderr, "%d byte : < %s > operands: %d\n",
            //      i->size(),
            //      i->format().c_str(),
            //      operands.size());
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
