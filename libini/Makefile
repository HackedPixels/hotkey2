CC := gcc
CFLAGS := -O2

libini.o: ini.o
	mv ini.o libini.o

clean:
	rm *.o *.so

%.o: %.c
	$(CC) $$DEBUG $(CFLAGS) -fpic -c $<
