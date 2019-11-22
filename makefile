CC_C = g++
CFLAGS = -Wall -g -std=c++11
LIBS= -lglut -lGL -lGLU

all: a1



%.o : %.c
	$(CC_C) -c $(CFLAGS) $<

a1: FruitTetris.o arm.o grid.o helper.o savedTetro.o tetris.o
	$(CC_C) -o FruitTetris FruitTetris.o arm.o grid.o helper.o savedTetro.o tetris.o $(LIBS)



clean:
	rm arm.o
	rm FruitTetris.o
	rm grid.o
	rm helper.o
	rm savedTetro.o
	rm tetris.o
	rm FruitTetris
