include ../config.mk

libgetopt.o: getopt.o
	mv getopt.o libgetopt.o

clean:
	rm -f *.o *.so

%.o: %.c
	$(CC) $$DEBUG $(CFLAGS) -fpic -c $<
