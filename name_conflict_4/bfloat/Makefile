.POSIX:

XCFLAGS = -O3 -pipe -march=native -lm

all:
	${CC} ${XCFLAGS} bfloat.c -c -o bfloat.o
	${CC} ${XCFLAGS} main.c -c -o main.o
	${CC} ${XCFLAGS} foo.c -c -o foo.o
	${CC} ${XCFLAGS} ${LDFLAGS} *.o -o main

clean:
	rm -f *.o main

.PHONY: all clean
