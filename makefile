COMPILER=g++
FLAGS=-Wall -O3
SOURCE=main.cpp
BIN=calculator

all:
	$(COMPILER) $(FLAGS) $(SOURCE) -o $(BIN)

clean:
	rm -rf $(BIN)