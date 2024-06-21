#ifndef MMN14_FILE_H
#define MMN14_FILE_H

#include "../data/boolean.h"
#include "error.h"
#include "../data/line.h"
#include "../assembler/translation/word/words_block.h"
#include "../assembler/translation/symbol/symbol_table.h"


typedef struct file{
    line_node *first_line;
    int *number_of_rows;
    char *filename;
} file;

void read_file(file *file1, error_array *error);
void print_pos(int line, int column);
void free_file_lines(file *file1);
void write_to_file_file(file file);
void write_to_file_object(word_list_block *block, char fileName[]);
void write_to_file_entry(symbol_table *symbol_table, char fileName[]);
void write_to_file_external(word_list_block *block, char fileName[], symbol_table *table);
void add_ending_to_file_name(char **fileName, error_array *error);

#endif
