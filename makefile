 CFLAGS = -ansi -Wall -pedantic -g
 EXE_DEPS = assembler.o post_formating.o first_pass.o second_pass.o word_conversion.o file.o error.o utilities.o symbol_table.o line_data.o consts.o # Deps for exe

 ## Executable
assembler: $(EXE_DEPS)
	gcc -g $(EXE_DEPS) $(CFLAGS) -o $@

assembler.o:  scripts
	gcc -c scripts/assembler.c $(CFLAGS) -o $@

post_formating.o: scripts Header
	gcc -c scripts/post_formating.c $(CFLAGS) -o $@

first_pass.o: scripts Header
	gcc -c scripts/first_pass.c $(CFLAGS) -o $@

second_pass.o: scripts Header
	gcc -c scripts/second_pass.c $(CFLAGS) -o $@

word_conversion.o: scripts Header
	gcc -c scripts/word_conversion.c $(CFLAGS) -o $@

file.o: scripts Header
	gcc -c scripts/file.c $(CFLAGS) -o $@

error.o: scripts Header
	gcc -c scripts/error.c $(CFLAGS) -o $@

utilities.o: scripts Header
	gcc -c scripts/utilities.c $(CFLAGS) -o $@

symbol_table.o: scripts Header
	gcc -c scripts/symbol_table.c $(CFLAGS) -o $@

line_data.o: scripts Header
	gcc -c scripts/line_data.c $(CFLAGS) -o $@

consts.o: scripts Header
	gcc -c scripts/consts.c $(CFLAGS) -o $@

clean:
	rm -rf *.o *.am *.ob *.ent *.ext