ltCXX=$(shell root-config --cxx)
CFLAGS=-c -g -Wall $(shell root-config --cflags) -I./src -I ./include
LDLIBS=$(shell root-config --glibs)
LDFLAGS=$(shell root-config --ldflags)
#SOURCES=./src/SL_Event.cc ./src/FileManager.cc ./src/Filter.cc

ROOTCINT=rootcint

NAME=LendaEvent
LIBRARY= $(addsuffix Lib.so,$(NAME))
DICTNAME=$(addsuffix Dictionary,$(NAME))
DICTOBJ=$(addsuffix .o,$(DICTNAME))
OBJECTS=$(addsuffix .o, $(NAME))

.PHONY: clean get put all test sclean

all: $(LIBRARY) 

$(LIBRARY) : $(DICTOBJ) $(OBJECTS)
	@echo "Building Library"
	@$(CXX) `root-config --cflags` -fPIC -shared -Wl -o $@ $^;
	@echo "Build succeed"
%Dictionary.o : %.hh %LinkDef.h
	@echo "Generating Dictionary"
	@$(ROOTCINT) -f $(patsubst %.o,%.cc,$@) -c $^;
	@echo "Compiling Dicionary"
	@$(CXX) -p -fPIC $(CFLAGS) -c $(patsubst %.o,%.cc,$@)

.cc.o:
	@echo "Compiling" $< "..."
	@$(CXX) $(CFLAGS) $< -o $@ 

test:
	@echo $(LIBRARY) : $(DICTOBJ) $(OBJECTS)    

clean:
	-rm $(DICTOBJ) $(OBJECTS)
	-rm *Dictionary.h
	-rm *Dictionary.cc
sclean:
	-rm *~
	make clean