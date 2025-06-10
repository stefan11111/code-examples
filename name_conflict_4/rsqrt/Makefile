XCC = ${CC} -O3 -march=skylake -lm

all:
	${XCC} rsqrt.c -c -o rsqrt.o
	${XCC} main.c -c -o main.o
	${XCC} *.o -o main

clean:
	rm -f *.o main

.PHONY: all clean
