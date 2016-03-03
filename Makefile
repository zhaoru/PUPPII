
# .SUFFIX = .so

CPP             = g++
CPPFLAGS        = -g -O1 -Wall -fPIC -D_REENTRANT -Wno-deprecated -I. 

ROOTCFLAGS      := $(shell root-config --cflags)
ROOTLIBS        := $(shell root-config --libs) -lMinuit -lEG #-lg2c
# ROOTLIBS        += $(shell echo ${MY_LDLib})
ROOTGLIBS       := $(shell root-config --glibs)

CPPFLAGS        += $(ROOTCFLAGS)
LIBS            = $(ROOTLIBS) -lm -L.
GLIBS           = $(ROOTGLIBS)

TARGET = main

OBJS =   base.o reader.o main.o analyzer.o

all: ${TARGET}
	echo "- - - - - - - - - - - - - - \n- - - - Successfully - - - - \n- - - - - - - - - - - - - -"

${TARGET}: $(OBJS)
	$(CPP) -o $@ $(CPPFLAGS) $(OBJS) $(LIBS) $(EXTERNAL_OBJS)

main.o :  main.cc  analyzer.h  base.h reader.h
	$(CPP) -c $(CPPFLAGS) main.cc

analyzer.o : analyzer.cc  analyzer.h  base.h reader.h
	$(CPP) -c $(CPPFLAGS) analyzer.cc

reader.o : reader.cc base.h reader.h
	$(CPP) -c $(CPPFLAGS) reader.cc

base.o : base.cc base.h 
	$(CPP) -c $(CPPFLAGS) base.cc

clean: 
	-rm ${TARGET}
	-rm ${OBJS}
	-rm *~
