#  Binary Rewriter
#  #  -------------------
#  #
#  Last Modified: Sept 30 
#  #  @author Stephen Holsapple
CC=gcc
CPP=g++
CFLAGS=-g -I$(HEADERS)
LDFLAGS= -L$(DYNINST) $(INCLUDES)
PLATFORM=i386-unknown-linux2.4
DYNINST_ROOT=..
INCLUDES=-ldyninstAPI -lsymtabAPI -linstructionAPI -lparseAPI -lcommon -liberty -ldwarf -lelf
TESTDIR=tests
HEADERS=../../include/rewritez/
DYNINST=../../lib/rewritez/i386-unknown-linux2.4/lib/

TESTS=test-read test-write
ALL=BinaryRewriter $(TESTS)
all:	$(ALL)

#Main application
BinaryRewriter: Main.o BinaryRewriter.o DataRewriter.o TextRewriter.o SymtabRewriter.o
	$(CPP) $(LDFLAGS) -o $@ $^

Main.o: Main.cpp
	$(CPP) $(CFLAGS) -c $<

BinaryRewriter.o: BinaryRewriter.cpp
	$(CPP) $(CFLAGS) -c $<

DataRewriter.o: DataRewriter.cpp
	$(CPP) $(CFLAGS) -c $<

TextRewriter.o: TextRewriter.cpp
	$(CPP) $(CFLAGS) -c $<

SymtabRewriter.o: SymtabRewriter.cpp
	$(CPP) $(CFLAGS) -c $<

tests:	$(TESTS)

# Tests
test-read: test-read.o
	$(CC) -o $@ $^
test-read.o: $(TESTDIR)/test-read.c
	$(CC) -g -c $<
test-write: test-write.o
	$(CC) -o $@ $^
test-write.o: $(TESTDIR)/test-write.c
	$(CC) -g -c $<

clean:
	rm -rf core* *.o *.gch *-mutated $(ALL)
