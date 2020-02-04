

ROOT := $(shell root-config --cflags) $(shell root-config --libs) -lEG

HIPO4 := $(HIPO)/hipo4

INCHIPO4 := $(HIPO4)
INCHIPO3 := $(HIPO3)

LIBHIPO4 := $(HIPO4)/../lib
LIBHIPO3 := $(HIPO3)/../lib

MACROS := $(HOME)/Macros

.PHONY:all
all:

hipo3: anClustersH3.C
	g++ -o anClustersH3 anClustersH3.C -I$(INCHIPO3) -L$(LIBHIPO3) -lhipo $(ROOT) -I$(MACROS) -llz4

cluster: anClusters.C
	g++ -o anClusters anClusters.C -I$(MACROS) -I$(INCHIPO4) -L$(LIBHIPO4) -lhipo4 $(ROOT) -llz4

lorentzAngle:
	g++ -o lorentzAngle lorentzAngle.C -I$(MACROS) -I$(INCHIPO4) -L$(LIBHIPO4) -lhipo4 $(ROOT) -llz4

.PHONY: clean
clean:
	rm anClustersH3 anClusters lorentzAngle
