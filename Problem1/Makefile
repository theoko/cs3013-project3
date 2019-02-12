CFLAGS = -pthread -Wall -g

all: pn

pn: pn.o
	gcc $(CFLAGS) pn.o -lm -o pn

pn.o:
	gcc $(CFLAGS) -c problem1.c

clean:
	rm -rf pn.o pn
