# wic MakeFile. 
# Targets: all (default, release), release, debug, doxygen, and clean.

# SETTINGS
CC         = g++
LD         = ld
CFLAGS     = -std=c++11
DEBUGFLAGS = -g

# You probably won't need to change anything beyond this point.
SOURCES       = $(wildcard src/*.cpp)
OBJECTS       = $(addprefix obj/release/,$(notdir $(SOURCES:.cpp=.o)))
DOBJECTS      = $(addprefix obj/debug/,$(notdir $(SOURCES:.cpp=.o)))
INCLUDEPATHS  = -I include/ -I deps/include/
COPTIONS      =

all: release
	
release: $(OBJECTS)
	ar -r bin/release/libwic.a obj/release/*.o deps/lib/*.o

obj/release/%.o: src/%.cpp
	mkdir -p bin/release/
	mkdir -p obj/release/
	$(CC) -O2 $(CFLAGS) $(COPTIONS) -c $< -o $@ $(INCLUDEPATHS)

debug: $(DOBJECTS)
	ar -r bin/debug/libwic.a obj/debug/*.o deps/lib/*.o

obj/debug/%.o: src/%.cpp
	mkdir -p bin/debug/
	mkdir -p obj/debug/
	$(CC) $(CFLAGS) $(DEBUGFLAGS) $(COPTIONS) -c $< -o $@ $(INCLUDEPATHS)

doxygen:
	doxygen docs/Doxyfile

clean:
	rm -f -r obj/release/*
	rm -f -r obj/debug/*
	rm -f -r bin/release/*
	rm -f -r bin/debug/*
	rm -f -r docs/html
	