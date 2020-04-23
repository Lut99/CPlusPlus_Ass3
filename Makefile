# MAKEFILE for C++ ASSIGNMENT 3                                               #
#   by Tim Müller (11774606)                                                  #
#                                                                             #
# g++ (Ubuntu 5.7.0-3ubuntu1~18.04) on KDE Neon (Ubuntu 18.04)                #

GXX=g++
GXX_ARGS=-O2 -Wall -Wextra

SRC=src
LIB=$(SRC)/lib
BIN=bin
OBJ=$(BIN)/obj

GXX_ARGS += -I $(LIB)/include


.PHONY: default all run main dirs clean
default: all

$(BIN):
	mkdir -p $(BIN)
$(OBJ):
	mkdir -p $(OBJ)

dirs: $(BIN) $(OBJ)

$(OBJ)/%.o: $(LIB)/%.cpp dirs
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
