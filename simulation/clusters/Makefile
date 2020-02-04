

ROOT := $(shell root-config --cflags) $(shell root-config --libs) -lEG

HIPO4 := /home/fbossu/software/Clas12/hipo/hipo4

INCHIPO4 := $(HIPO4)

LIBHIPO4 := $(HIPO4)/../lib

MACROS := $(HOME)/Macros

all: hipo4

hipo4: cluster

cluster:
	g++ -o anClusters anClusters.C -I$(MACROS) -I$(INCHIPO4) -L$(LIBHIPO4) -lhipo4 $(ROOT) -llz4

lorentzAngle:
	g++ -o lorentzAngle lorentzAngle.C -I$(MACROS) -I$(INCHIPO4) -L$(LIBHIPO4) -lhipo4 $(ROOT) -llz4

#hipo4:
	#g++  -o anEffHipo4 anEff_Hipo4.C -I$(INCHIPO4) -L$(LIBHIPO4) -lhipo4 $(ROOT)
