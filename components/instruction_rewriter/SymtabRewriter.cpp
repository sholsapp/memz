#include <cstdio>
#include <string>
#include <algorithm>
#include <vector>

#include "SymtabRewriter.h"

SymtabRewriter::SymtabRewriter(Symtab* _symTab, AddrMapping* reloc) {
   /**
    *
    */
   symTab = _symTab;

   //VarList allVars;
   //if (symTab->getAllVariables(allVars)) {
   //   for (VarList::iterator it = allVars.begin(); it != allVars.end(); ++it) {
   //      Variable* var = *it;
   //      SymList sym;
   //      if (var->getSymbols(sym)) {
   //         for (SymList::iterator it = sym.begin(); it != sym.end(); ++it) {
   //            Symbol* s = *it;
   //            fprintf(stderr, "Pretty name: %s ", s->getPrettyName().c_str());
   //            fprintf(stderr, "in %s ", s->getRegion()->getRegionName().c_str());
   //            fprintf(stderr, "at %p\n", (void*) s->getOffset());
   //
   //           }
   //       }
   //   }
   //}

   SymList objSyms;
   SymList regSyms;

   if(!symTab->getAllSymbolsByType(objSyms, Symbol::ST_OBJECT)) {
      fprintf(stderr, "Couldn't fetch ST_OBJECT symbols.\n");
   }

   if(!symTab->getAllSymbolsByType(regSyms, Symbol::ST_SECTION)) {
      fprintf(stderr, "Couldn't fetch ST_SECTION symbols.\n");
   }


   for (SymList::iterator it = objSyms.begin();
         it != objSyms.end();
         ++it) {
      Symbol* s = *it;
      fprintf(stderr, "ST_OBJECT: %s, offset = %p\n", s->getPrettyName().c_str(), (void*) s->getOffset());

      if ((*reloc)[s->getOffset()] != 0) {
         fprintf(stderr, "Changing %p to %p\n", s->getOffset(), (*reloc)[s->getOffset()]);
         s->setOffset((Offset) (*reloc)[s->getOffset()]);
      }

   }

   for (SymList::iterator it = regSyms.begin();
         it != regSyms.end();
         ++it) {
      Symbol* s = *it;
      fprintf(stderr, "ST_SECTION: %s, offset = %p, section = %s\n", s->getPrettyName().c_str(), (void*) s->getOffset(), s->getRegion()->getRegionName().c_str());
      if ((*reloc)[s->getOffset()] != 0) {
         fprintf(stderr, "Changing %p to %p\n", s->getOffset(), (*reloc)[s->getOffset()]);

         /*
         if (s->isAbsolute()) {
            fprintf(stderr, "Symbol is absolute\n");
         }

         if (s->isInDynSymtab()) {
            fprintf(stderr, "Symbol is in dyn table\n");
         }

         if (s->isInSymtab()) {
            fprintf(stderr, "Symbol is in table\n");
         }
         */

         if(!s->setOffset((Offset) (*reloc)[s->getOffset()])) {
            fprintf(stderr, "Sym mod failed\n");
         }

      }

   }

   return;
}
