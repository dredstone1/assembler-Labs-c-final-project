myset: compiler.o
	gcc -g -ansi -pedantic -Wall compiler.o -o compiler

compiler.o: scripts/compiler.c scripts/compiler.h
	gcc -c -g -ansi -pedantic -Wall compiler.c -o compiler.o

exec:
	./compiler

