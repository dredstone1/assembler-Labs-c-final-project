CFLAGS = -ansi -Wall -pedantic -g
EXE_DEPS = assembler.o post_formating.o first_pass.o second_pass.o word_conversion.o file.o error.o utilities.o symbol_table.o line_data.o consts.o
FOLDERS = header scripts


## Executable
assembler: $(EXE_DEPS)
	gcc -g $(EXE_DEPS) $(CFLAGS) -o $@

assembler.o:  scripts
	gcc -c scripts/assembler.c $(CFLAGS) -o $@

post_formating.o: $(FOLDERS)
	gcc -c scripts/post_formating.c $(CFLAGS) -o $@

first_pass.o: $(FOLDERS)
	gcc -c scripts/first_pass.c $(CFLAGS) -o $@

second_pass.o: $(FOLDERS)
	gcc -c scripts/second_pass.c $(CFLAGS) -o $@

word_conversion.o: $(FOLDERS)
	gcc -c scripts/word_conversion.c $(CFLAGS) -o $@

file.o: $(FOLDERS)
	gcc -c scripts/file.c $(CFLAGS) -o $@

error.o: $(FOLDERS)
	gcc -c scripts/error.c $(CFLAGS) -o $@

utilities.o: $(FOLDERS)
	gcc -c scripts/utilities.c $(CFLAGS) -o $@

symbol_table.o: $(FOLDERS)
	gcc -c scripts/symbol_table.c $(CFLAGS) -o $@

line_data.o: $(FOLDERS)
	gcc -c scripts/line_data.c $(CFLAGS) -o $@

consts.o: $(FOLDERS)
	gcc -c scripts/consts.c $(CFLAGS) -o $@

## Clean
clean:
	rm -rf *.o
	
## clean all
cleanALL:
	rm -rf *.o *.am *.ob *.ent *.ext