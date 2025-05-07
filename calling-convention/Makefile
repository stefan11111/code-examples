XCFLAGS = -Wno-builtin-declaration-mismatch ${CFLAGS}

all:
	${CC} ${XCFLAGS} ${LDFLAGS} calling-convention.c -o calling-convention
	${CC} -O3 ${XCFLAGS} foo.c -c -o foo.o
	${CC} -O3 ${XCFLAGS} main.c -c -o main.o
	${CC} ${XCFLAGS} ${LDFLAGS} *.o -o main

clean:
	rm -f foo.o main.o main calling-convention
