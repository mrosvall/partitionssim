# Various flags

CXX  = g++-5
LINK = $(CXX)
#CXXFLAGS = -Wall -g 
CXXFLAGS = -std=c++11 -Wall -O3 -fopenmp
LFLAGS = -lm -fopenmp


TARGET  = partitionssim

HEADER  = partitionssim.h
FILES = partitionssim.cc

OBJECTS = $(FILES:.cc=.o)

$(TARGET): ${OBJECTS}
	$(LINK) $^ $(LFLAGS) -o $@

all: $(TARGET)

clean:
	rm -f $(OBJECTS)

distclean:
	rm -f $(OBJECTS) $(TARGET)

# Compile and dependency
$(OBJECTS): $(HEADER) Makefile




