CFLAGS=-O3 -mavx -Wall
CC=gcc

mm: main.o vectors.o
	gcc main.o vectors.o -o mm

clean:
	rm main.o vectors.o mm

perf: mm FORCE
	perf stat -e cycles -e instructions ./mm 1999

check: mm FORCE
	./mm 1999
	diff -s matmul.out matmul.1999x1999x99

dist:
	mkdir effizienz-aufgabe17
	cp -p main.c vectors.c vectors.h Makefile HEADER.html matmul.1999x1999x99 effizienz-aufgabe17
	tar cfz effizienz-aufgabe17.tar.gz effizienz-aufgabe17
	rm -rf effizienz-aufgabe17

vectors.o: vectors.c vectors.h

main.o: main.c vectors.h

FORCE:

