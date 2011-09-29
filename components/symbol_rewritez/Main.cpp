/**
 * Main program
 */

#include "Boa.h"

#include <cstdio>

#include <BPatch.h>
#include <BPatch_function.h>

int main(int argc, char** argv) {

   if (argc < 2) {
      fprintf(stdout, "Usage: ./program <binary to rewrite>\n");
      exit(EXIT_SUCCESS);
   }

   /*

   BPatch bpatch;
   BPatch_binaryEdit* binedit = bpatch.openBinary(argv[1]);

   BPatch_image* appImage = binedit->getImage();

   std::vector<BPatch_function*> funcs;
   appImage->findFunction("main", funcs);

   BPatch_flowGraph* fg = funcs[0]->getCFG();

   std::set<BPatch_basicBlock*> blocks;
   fg->getAllBasicBlocks(blocks);

   std::set<BPatch_basicBlock*>::iterator block_iter;
   for (block_iter = blocks.begin(); block_iter != blocks.end(); ++block_iter) {

      BPatch_basicBlock* block = *block_iter;
      std::vector<Dyninst::InstructionAPI::Instruction::Ptr> insns;
      block->getInstructions(insns);
      std::vector<Dyninst::InstructionAPI::Instruction::Ptr>::iterator insn_iter;

      for (insn_iter = insns.begin(); insn_iter != insns.end(); ++insn_iter) {

         Dyninst::InstructionAPI::Instruction::Ptr insn = *insn_iter;

         //if (insn->readsMemory() || insn->writesMemory()) {
         //   //insns_access_memory++;
         //   fprintf(stderr, "Accessed memory\n");
         //}

         fprintf(stderr, "Inst: %s\n", insn->format().c_str());
         fprintf(stderr, "Instruction size: %d\n", insn->size());
         fprintf(stderr, "Addr of raw ptr: %x\n", insn->ptr());

         std::vector<Dyninst::InstructionAPI::Operand> operands;

         insn->getOperands(operands);

         for (int i = 0; i < operands.size(); ++i) {
           fprintf(stderr, "operand = %s\n", operands[i].format().c_str());
         }


      }
   }

   */


   // The constructor does all the work currently
   Boa boa(argv[1]);

   return 0;
}



