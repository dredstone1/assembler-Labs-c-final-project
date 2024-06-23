myset: assembler.o
	gcc -g -ansi -pedantic -Wall assembler.o -o assembler

assembler.o: scripts/assembler.c scripts/assembler.h
	gcc -c -g -ansi -pedantic -Wall compiler.c -o compiler.o

exec:
	./assembler

