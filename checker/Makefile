CC = g++
CFLAGS = -Wall -g

all: build

build: sync lab

sync: tema.o
	$(CC) $(CFLAGS) -o $@ $^
	
lab: lab.o
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

run: build
	./sync $(problem)

labyrinth: build
	./lab

clean:
	rm -f *.o *~ sync lab
