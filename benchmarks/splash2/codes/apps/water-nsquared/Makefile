TARGET = WATER-NSQUARED
HOARD_TARGET = WATER-NSQUARED-HOARD

ALL = $(TARGET) $(HOARD_TARGET)

OBJS = water.o mdmain.o syscons.o kineti.o bndry.o cnstnt.o initia.o interf.o intraf.o cshift.o poteng.o predcor.o
CFLAGS = -O2
LDFLAGS = -lm 
MACROS = ../../null_macros/c.m4.null

HOARD_SO_PATH=../../..
HOARD = -L$(HOARD_SO_PATH) -lhoard

.H.h:
	m4 $(MACROS) $*.H > $*.h
.C.c:
	m4 $(MACROS) $*.C > $*.c
.c.o:
	cc -c $(CFLAGS) $*.c
.C.o:
	m4 $(MACROS) $*.C > $*.c
	cc -c $(CFLAGS) $*.c

$(TARGET): $(OBJS)
	cc $(OBJS) $(CFLAGS) -o $(TARGET) $(LDFLAGS)

$(HOARD_TARGET): $(OBJS)
	cc $(OBJS) $(CFLAGS) -o $(HOARD_TARGET) $(LDFLAGS) $(HOARD)

all: $(ALL)

clean:
	rm -rf *.o $(ALL)

cnstnt.c: cnstnt.C water.h wwpot.h cnst.h frcnst.h fileio.h parameters.h
water.c: water.C parameters.h mdvar.h water.h wwpot.h cnst.h mddata.h fileio.h frcnst.h randno.h global.h split.h
mdmain.c: mdmain.C  parameters.h mdvar.h water.h wwpot.h cnst.h  mddata.h fileio.h frcnst.h randno.h global.h split.h
syscons.c: syscons.C parameters.h mdvar.h water.h wwpot.h cnst.h mddata.h
kineti.c: kineti.C global.h split.h mdvar.h parameters.h mddata.h
bndry.c: bndry.C split.h mdvar.h parameters.h mddata.h
initia.c: initia.C split.h mdvar.h water.h cnst.h fileio.h parameters.h mddata.h
predcor.c: predcor.C split.h mdvar.h parameters.h mddata.h
intraf.c: intraf.C frcnst.h mdvar.h water.h wwpot.h parameters.h mddata.h global.h split.h
interf.c: interf.C mdvar.h water.h wwpot.h cnst.h global.h split.h parameters.h mddata.h
cshift.c: cshift.C water.h
poteng.c: poteng.C global.h split.h mdvar.h frcnst.h water.h wwpot.h parameters.h mddata.h

