.POSIX:
.SUFFIXES: .c .h .o

OBJ = foo.o bar.o
HDR = foo.h

all: ${OBJ}

${OBJ} : ${HDR}

.c.o:
	${CC} ${XCFLAGS} -c -o $@ $<

clean:
	rm ${OBJ}

.PHONY: all clean

