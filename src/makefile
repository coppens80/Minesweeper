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
    -$(DEBUG)

override LDFLAGS += \
	-lsfml-graphics \
	-lsfml-window \
	-lsfml-system

DEBUG += g \
		-Wextra 
		
TARGETS = test

DEPS = $(shell find . -name '*.hpp' -or -name '*.h' -or -name '*.hh') 

.PHONY: all
all: $(TARGETS)

test: main.o
	$(CXX) -o $@ $^ $(LDFLAGS)

main.o:  $(DEPS)

.PHONY: clean
clean:
	@find . -name '*.o' -exec rm {} \;
	@rm -f $(TARGETS)

