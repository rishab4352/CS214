CFLAGS= -g

all: memgrind clean

memgrind: memgrind.c mymalloc.o
	gcc $(CFLAGS) -o memgrind memgrind.c mymalloc.o

mymalloc.o: mymalloc.c mymalloc.h
	gcc $(CFLAGS) -c mymalloc.c

clean:
	-rm -f *.o
	-rm -f *.h.gch