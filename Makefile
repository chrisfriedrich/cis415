CC = gcc
CFLAGS = -W -Wall
PROGS = uspsv1 uspsv2 uspsv3 uspsv4 iotest cputest

all: $(PROGS)

uspsv1: uspsv1.o p1fxns.o
	cc -o uspsv1 uspsv1.o p1fxns.o

uspsv2: uspsv2.o p1fxns.o
	cc -o uspsv2 uspsv2.o p1fxns.o

uspsv3: uspsv3.o p1fxns.o
	cc -o uspsv3 uspsv3.o p1fxns.o

uspsv4: uspsv4.o p1fxns.o
	cc -o uspsv4 uspsv4.o p1fxns.o

iotest: iotest.o 
	cc -o iotest iotest.o

cputest: cputest.o
	cc -o cputest cputest.o

uspsv1.o: uspsv1.c p1fxns.h
uspsv2.o: uspsv2.c p1fxns.h
uspsv3.o: uspsv3.c p1fxns.h
uspsv4.o: uspsv4.c p1fxns.h

iotest.o: iotest.c
cputest.o: cputest.c

clean:

	rm - f *~ *.o $(PROGS)
