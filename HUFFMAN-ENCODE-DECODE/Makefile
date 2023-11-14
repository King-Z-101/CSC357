CC = gcc

CFLAGS = -Wall -g

LD = gcc

OBJS = htable.o 

all: hencode hdecode

htable: $(OBJS)
	$(CC) $(CFLAGS) -o htable $(OBJS)

hencode: hencode.o utility.o
	$(CC) $(CFLAGS) -o hencode hencode.o utility.o

hencode.o: hencode.c utility.h
	$(CC) $(CFLAGS) -c hencode.c

hdecode: hdecode.o utility.o
	$(CC) $(CFLAGS) -o hdecode hdecode.o utility.o

hdecode.o: hdecode.c utility.h
	$(CC) $(CFLAGS) -c hdecode.c

utility.o: utility.c utility.h 
	$(CC) $(CFLAGS) -c utility.c


test: all
	~pn-cs357/demos/htable test1 > test.out
	./htable test1 > test.correct.out 
	diff test.out test.correct.out
	rm -f test.out test.correct.out

test1: all
	echo Testing all
	./htable test1
	echo done.

test2: all
	echo Testing all
	./htable test2
	echo done.

test3: all
	echo Testing all
	./htable test3
	echo done.

test4: all
	echo Testing all
	./htable test4
	echo done.

test5: all
	echo Testing all
	./htable test5
	echo done.

clean:
	rm *.o 
	rm htable
