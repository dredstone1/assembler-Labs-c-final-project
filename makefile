# Compilation macros
CC = gcc
CFLAGS = -ansi -Wall -pedantic -g # Flags

# Dependencies
GLOBAL_DEPS = 
EXE_DEPS = assembler.o error.o file.o first_pass.o line_data.o line_node.o macro.o post_formating.o symbol_table.o utilitis.o words_block.o # Deps for exe

# Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

# Object files
assembler.o: scripts/assembler.c header/post_formating.h header/first_pass.h header/file.h header/error.h header/words_block.h header/symbol_table.h header/line_data.h header/line_node.h header/utilities.h
	$(CC) -c scripts/assembler.c $(CFLAGS) -o $@

error.o: scripts/error.c header/error.h header/utilities.h header/line_node.h
	$(CC) -c scripts/error.c $(CFLAGS) -o $@

file.o: scripts/file.c header/file.h header/utilities.h header/error.h header/words_block.h header/symbol_table.h header/line_node.h
	$(CC) -c scripts/file.c $(CFLAGS) -o $@

first_pass.o: scripts/first_pass.c header/first_pass.h header/file.h header/error.h header/words_block.h header/symbol_table.h header/line_data.h header/line_node.h
	$(CC) -c scripts/first_pass.c $(CFLAGS) -o $@

line_data.o: scripts/line_data.c header/line_data.h header/symbol_table.h header/error.h
	$(CC) -c scripts/line_data.c $(CFLAGS) -o $@

line_node.o: scripts/line_node.c header/line_node.h
	$(CC) -c scripts/line_node.c $(CFLAGS) -o $@

macro.o: scripts/macro.c header/macro.h header/error.h header/line_node.h header/utilities.h
	$(CC) -c scripts/macro.c $(CFLAGS) -o $@

post_formating.o: scripts/post_formating.c header/file.h header/macro.h header/error.h header/line_node.h
	$(CC) -c scripts/post_formating.c $(CFLAGS) -o $@

symbol_table.o: scripts/symbol_table.c header/symbol_table.h header/utilities.h header/error.h
	$(CC) -c scripts/symbol_table.c $(CFLAGS) -o $@

utilitis.o: scripts/utilities.c header/utilities.h
	$(CC) -c scripts/utilitis.c $(CFLAGS) -o $@

words_block.o: scripts/words_block.c header/words_block.h header/symbol_table.h header/line_data.h header/error.h
	$(CC) -c scripts/words_block.c $(CFLAGS) -o $@

# Clean rule
clean:
	rm -rf *.o *.am *.ob *.ent *.ext