CC=g++
CFLAGS=--std=c++11

all: 2048

2048: board.h board.cpp main.h main.cpp
	  $(CC) $(CFLAGS) board.cpp main.cpp -o 2048
clean:
	  rm 2048