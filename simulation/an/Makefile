

ROOT := $(shell root-config --cflags) $(shell root-config --libs) -lEG

HIPO4 := /home/fbossu/software/Clas12/hipo/hipo4
HIPO3 := /home/fbossu/software/Clas12Tool/Hipo3

INCHIPO4 := $(HIPO4)
INCHIPO3 := $(HIPO3)

LIBHIPO4 := $(HIPO4)/../lib
LIBHIPO3 := $(HIPO3)/../lib

.PHONY: all
all: hipo4

hipo3:
	g++ -o anEffHipo3 anEff_Hipo3.C -I$(MACROS) -I$(INCHIPO3) -L$(LIBHIPO3) -lhipo $(ROOT) -llz4
	g++ -o tracks3 tracks3.C -I$(MACROS) -I$(INCHIPO3) -L$(LIBHIPO3) -lhipo $(ROOT) -llz4

hipo4:
	g++  -o anEffHipo4 anEff_Hipo4.C -I$(MACROS) -I$(INCHIPO4) -L$(LIBHIPO4) -lhipo4 $(ROOT)  -llz4
