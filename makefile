myset: compiler.o
	gcc -g -ansi -pedantic -Wall compiler.o -o compiler

compiler.o: compiler.c compiler.h
	gcc -c -g -ansi -pedantic -Wall compiler.c -o compiler.o

exec:
	./compiler

