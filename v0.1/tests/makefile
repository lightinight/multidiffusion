PROGS1= test_parameter

MAIN1= test_parameter.cpp 

INCLUDES= -I../include

CFLAGS= -Wall -std=c++11

CC=g++

all: $(PROGS1) 

$(PROGS1):$(MAIN1) $(LIBSRC) Makefile
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROGS1) $(MAIN1)

clean:
	rm -f *.bak *.o .g* .d* $(PROGS1)

