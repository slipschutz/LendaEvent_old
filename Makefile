ltCXX=$(shell root-config --cxx)
CFLAGS=-c -g -Wall $(shell root-config --cflags) -I./src -I ./include
LDLIBS=$(shell root-config --glibs)
LDFLAGS=$(shell root-config --ldflags)
#SOURCES=./src/SL_Event.cc ./src/FileManager.cc ./src/Filter.cc

ROOTCINT=rootcint

NAME=LendaEvent
temp= $(addprefix lib,$(NAME))
temp2=$(addsuffix $(LDFLAGS),$(temp))
LIBRARY= $(addsuffix .so,$(temp2))
DICTNAME=$(addsuffix Dictionary,$(NAME))
DICTOBJ=$(addsuffix .o,$(DICTNAME))
OBJECTS=$(addsuffix .o, $(NAME))

.PHONY: clean get put all test sclean

all: $(LIBRARY) 

$(LIBRARY) : $(DICTOBJ) $(OBJECTS)
	@echo "Building Library"
	@$(CXX) `root-config --cflags` -fPIC -shared -o $@ $^;
	@echo "Build succeed"

%Dictionary.o : %.hh %LinkDef.h
	@echo "Generating Dictionary"
	@$(ROOTCINT) -f $(patsubst %.o,%.cc,$@) -c $^;
	@echo "Compiling Dicionary"
	@$(CXX) -p -fPIC $(CFLAGS) -c $(patsubst %.o,%.cc,$@)

.cc.o:
	@echo "Compiling" $< "..."
	@$(CXX) $(CFLAGS) -fPIC $< -o $@ 

test:
	@echo $(LIBRARY) : $(DICTOBJ) $(OBJECTS)    

clean:
	-rm -f $(DICTOBJ) $(OBJECTS)
	-rm -f *Dictionary.h
	-rm -f *Dictionary.cc
	-rm -f $(LIBRARY)
sclean:
	-rm *~
	make clean