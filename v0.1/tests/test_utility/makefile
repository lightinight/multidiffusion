PROGS1= test_diffusivity

MAIN1= test_diffusivity.cpp
#test_parameter.cpp test_mobility.cpp

INCLUDES= -I../../include

CFLAGS= -O3 -Wall -std=c++11 -g

LDFLAGS= -L../../lib

CC=g++

STDLIBS= -lm
LIBSRC= ../../lib/libmultid.a

all: $(PROGS1) 

$(PROGS1):$(MAIN1) $(LIBSRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(PROGS1) $(MAIN1) $(LIBSRC) $(LDFLAGS) $(STDLIBS)

clean:
	rm -f *.bak *.o .g* .d* $(PROGS1)

