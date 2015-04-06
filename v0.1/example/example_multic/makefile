CC=g++
CFLAGS=-c -Wall -std=c++11

all: test

multidiffusion.o: multidiffusion.cpp multidiffusion.h
	$(CC) $(CFLAGS) multidiffusion.cpp

example.o: example.cpp multidiffusion.h
	$(CC) $(CFLAGS) example.cpp

test: example.o multidiffusion.o
	$(CC) example.o multidiffusion.o -o test
	
clean:
	rm *.o
