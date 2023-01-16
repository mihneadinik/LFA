CC = g++
CFLAGS = -Wall -g

all: build

build: sync

sync: tema.o
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

run: build
	./sync $(problem)

labyrinth: build

clean:
	rm -f *.o *~ sync
