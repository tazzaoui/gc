CC=gcc
CFLAGS=-g
EXEC=gc-test

gc-test: gc.o main.o
	$(CC) $(CFLAGS) main.o gc.o -o gc-test

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

gc.o: gc.c gc.h
	$(CC) $(CFLAGS) -c gc.c -o gc.o

clean:
	rm -f $(EXEC) *.o *~
