include ../config.mk

libini.o: ini.o
	mv ini.o libini.o

clean:
	rm -f *.o *.so

%.o: %.c
	$(CC) $$DEBUG $(CFLAGS) -fpic -c $<
