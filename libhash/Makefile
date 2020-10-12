include ../config.mk

libhash.o: hash.o
	mv hash.o libhash.o

clean:
	rm -f *.o *.so

%.o: %.c
	$(CC) $$DEBUG $(CFLAGS) -fpic -c $<
