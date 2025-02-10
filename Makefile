CC = g++
CPPFLAGS = -std=c++17 -Wall -g -Iinclude
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
EXEC = game

$(EXEC): $(OBJ)
	$(CC) $(CPPFLAGS) -o $(EXEC) $(OBJ) $(LDFLAGS)

objects: $(OBJ)

build/%.o: src/%.cpp | build
	$(CC) $(CPPFLAGS) -c $< -o $@

build:
	mkdir -p build

.PHONY: run clean

run:
	./$(EXEC)

clean:
	rm -rf build $(EXEC)
