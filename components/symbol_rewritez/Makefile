#  @author sholsapp
CC=gcc
CPP=g++
CFLAGS=-g
LDFLAGS=
PLATFORM=i386-unknown-linux2.4
DYNINST_ROOT=..
INCLUDES=-ldyninstAPI -lsymtabAPI -linstructionAPI -lparseAPI -lcommon -liberty -ldwarf -lelf
HEADERS=../include
DYNINST=../i386-unknown-linux2.4/lib

ALL=Main test
 
all: $(ALL)
 
Boa.o: Boa.cpp
	$(CPP) -c $(CFLAGS) -I$(HEADERS) Boa.cpp 

Main.o: Main.cpp
	$(CPP) -c $(CFLAGS) -I$(HEADERS) Main.cpp
 
Main: Main.o Boa.o
	$(CPP) Main.o Boa.o  -L$(DYNINST) $(INCLUDES) -o Main

test:
	gcc -O0 -g test.c -o original	
 
clean:
	rm -rf core* *.o *.gch original original-mutated $(ALL)
