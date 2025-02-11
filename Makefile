CC = g++
CPPFLAGS = -std=c++17 -Wall -g -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
EXEC = game

PCH = include/stdafx.hpp
PCH_GCH = $(PCH).gch
PCH_OBJ = build/stdafx.o

$(EXEC): $(OBJ)
	$(CC) $(CPPFLAGS) -o $(EXEC) $(PCH_OBJ) $(OBJ) $(LDFLAGS)

objects: $(OBJ)

$(PCH_OBJ): src/stdafx.cpp | build $(PCH_GCH)
	$(CC) $(CPPFLAGS) -c $< -o $@

$(PCH_GCH): $(PCH)
	$(CC) $(CPPFLAGS) -c $(PCH) -o $(PCH_GCH)

build/%.o: src/%.cpp | build $(PCH_GCH)
	$(CC) $(CPPFLAGS) -c $< -o $@

build:
	mkdir -p build

.PHONY: run clean

run:
	./$(EXEC)

clean:
	rm -rf build $(EXEC) $(PCH_GCH)
