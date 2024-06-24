CC=gcc
CFLAGS=-I.

%.o: %.c 
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o
	$(CC) -o obj/main main.o

run: main
	./obj/main