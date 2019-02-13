CFLAGS = -pthread -Wall -g

all: pn

pn: pn.o
	gcc $(CFLAGS) problem1.o -lm -o pn

pn.o:
	gcc $(CFLAGS) -c problem1.c

clean:
	rm -rf *.o pn
