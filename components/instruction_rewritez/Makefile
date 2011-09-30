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
HEADERS=../../include/rewritez/
DYNINST=../../lib/rewritez/i386-unknown-linux2.4/lib/

TESTS=test-read test-write
ALL=rewriter $(TESTS)
all:	$(ALL)

#Main application
rewriter: rewriter.o
	$(CPP) $(LDFLAGS) -o $@ $^

rewriter.o: rewriter.cpp
	$(CPP) $(CFLAGS) -c $<

tests:	$(TESTS)

# Tests
test-read: test-read.o
	$(CC) $(LDFLAGS) -o $@ $^
test-read.o: test-read.c
	$(CC) $(CFLAGS) -c $<
test-write: test-write.o
	$(CC) $(LDFLAGS) -o $@ $^
test-write.o: test-write.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf core* *.o *.gch $(ALL)
