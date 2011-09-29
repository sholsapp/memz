#  @author sholsapp
CC=gcc
CPP=g++
CFLAGS=-g
LDFLAGS=
PLATFORM=i386-unknown-linux2.4
DYNINST_ROOT=..
INCLUDES=-ldyninstAPI -lsymtabAPI -linstructionAPI -lparseAPI -lcommon -liberty -ldwarf -lelf
HEADERS=../../include/rewritez/
DYNINST=../../lib/rewritez/i386-unknown-linux2.4/lib/


ALL=instr_rewritez test
 
all: $(ALL)
 
Main.o: Main.cpp
	$(CPP) -c $(CFLAGS) -I$(HEADERS) Main.cpp
 
instr_rewritez: Main.o 
	$(CPP) Main.o -L$(DYNINST) $(INCLUDES) -o instr_rewritez

test:
	gcc -O0 -g test.c -o original	
 
clean:
	rm -rf core* *.o *.gch original original-mutated $(ALL)
