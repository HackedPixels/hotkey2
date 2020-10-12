CC := gcc
CFLAGS := -O2

libhash.o: hash.o
	mv hash.o libhash.o

clean:
	rm *.o *.so

%.o: %.c
	$(CC) $$DEBUG $(CFLAGS) -fpic -c $<
