GXX=g++
GXX_ARGS=-O2 -Wall -Wextra

SRC=src
LIB=$(SRC)/lib
BIN=bin
OBJ=$(BIN)/obj
TST=tests
TST_BIN=$(BIN)/tests

GXX_ARGS += -I $(LIB)/include


.PHONY: default all run main clean
default: all

$(OBJ)/%.o: $(LIB)/%.cpp
	$(GXX) $(GXX_ARGS) -o $@ -c $<

$(BIN)/main.out: $(SRC)/main.cpp $(OBJ)/World.o $(OBJ)/Menu.o $(OBJ)/Params.o $(OBJ)/Random.o
	$(GXX) $(GXX_ARGS) -o $@ $< $(OBJ)/World.o $(OBJ)/Menu.o $(OBJ)/Params.o $(OBJ)/Random.o

main: $(BIN)/main.out

run: main
	bin/main.out

all: main
clean:
	rm -f $(BIN)/*.out
	rm -f $(OBJ)/*.o
	rm -f $(TST_BIN)/*.out
