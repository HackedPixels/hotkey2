CC := gcc
CFLAGS := -O2

libgetopt.o: getopt.o
	mv getopt.o libgetopt.o

clean:
	rm *.o *.so

%.o: %.c
	$(CC) $$DEBUG $(CFLAGS) -fpic -c $<
