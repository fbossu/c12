

ROOT := $(shell root-config --cflags) $(shell root-config --libs) -lEG

HIPO4 := $(HIPO)/hipo4

INCHIPO4 := $(HIPO4)
INCHIPO3 := $(HIPO3)

LIBHIPO4 := $(HIPO4)/../lib
LIBHIPO3 := $(HIPO3)/../lib

MACROS := $(HOME)/Macros

.PHONY:all
all:
	g++ -o anHits anHits.C -I$(MACROS) -I$(INCHIPO4) -L$(LIBHIPO4) -lhipo4 $(ROOT) -llz4


.PHONY: clean
clean:
	rm anHits
