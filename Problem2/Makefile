CFLAGS = -pthread -Wall -g

all: pn

pn: pn.o
	gcc $(CFLAGS) problem2.o -lm -o drive

pn.o:
	gcc $(CFLAGS) -c problem2.c

clean:
	rm -rf *.o drive
