

ROOT := $(shell root-config --cflags) $(shell root-config --libs) -lEG

HIPO4 := /home/fbossu/software/Clas12/hipo/hipo4

INCHIPO4 := $(HIPO4)

LIBHIPO4 := $(HIPO4)/../lib

INC := $(HOME)/Macros

all: hipo4

hipo4:
	g++  -o twoProngs twoProngs.C -I$(INC) -I$(INCHIPO4) -L$(LIBHIPO4) -lhipo4 $(ROOT) -llz4
