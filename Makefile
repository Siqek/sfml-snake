CC = g++
CPPFLAGS = -std=c++17 -Wall -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

DEBUG ?= false

ifeq ($(DEBUG), true)
CPPFLAGS += -g -O0
else
CPPFLAGS += -O2 -DNDEBUG
endif

SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
EXEC = game

PCH = include/stdafx.hpp
PCH_GCH = $(PCH).gch
PCH_OBJ = build/stdafx.o

SRCDIRS = $(wildcard src/**/)
BUILDDIRS = $(patsubst src/%, build/%, $(SRCDIRS))

$(EXEC): $(OBJ)
	$(CC) $(CPPFLAGS) -o $(EXEC) $(PCH_OBJ) $(OBJ) $(LDFLAGS)

objects: $(OBJ)

$(PCH_OBJ): src/stdafx.cpp | build $(PCH_GCH)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PCH_GCH): $(PCH)
	$(CC) $(CPPFLAGS) -c $(PCH) -o $(PCH_GCH)

build/%.o: src/%.cpp | build $(BUILDDIRS) $(PCH_GCH)
	$(CC) $(CPPFLAGS) -c $< -o $@

build/%/:
	mkdir -p $@

build:
	mkdir -p build

.PHONY: run clean debug release

run:
	./$(EXEC)

clean:
	rm -rf build $(EXEC) $(PCH_GCH)

debug:
	make DEBUG=true

release:
	make DEBUG=false
