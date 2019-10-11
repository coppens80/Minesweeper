override DEPPATH = ../dep

CXX := g++
NVCC := nvcc -ccbin /usr/bin
LINK := g++ -fPIC

INCLUDES = \
    -I. 

override CXXFLAGS += \
    -O3 \
    -std=c++17 \
    -fpermissive \
    -Wall 

override LDFLAGS += \
	-lsfml-graphics \
	-lsfml-window \
	-lsfml-system

TARGETS = test

DEPS = version.h $(shell find . -name '*.hpp' -or -name '*.h' -or -name '*.hh') 

.PHONY: all
all: $(TARGETS)

test: minesweepy.o
	$(CXX) -o $@ $^ $(LDFLAGS)

propose.o:  $(DEPS)

.PHONY: clean
clean:
	@find . -name '*.o' -exec rm {} \;
	@rm -f $(TARGETS)

