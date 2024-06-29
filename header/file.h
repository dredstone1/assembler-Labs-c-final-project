#ifndef MMN14_FILE_H
#define MMN14_FILE_H

#include "utilities.h"
#include "error.h"
#include "words_block.h"
#include "symbol_table.h"
#include "line_node.h"


typedef struct file{
    line_node *first_line;
    int number_of_rows;
    char *filename;
} file;

void read_file(file *file1, error_array *error);
void free_file_lines(file *file1);
void write_to_file_file(file file);
void write_to_file_object(word_list_block *block, char fileName[]);
void write_to_file_entry(symbol_table *symbol_table, char fileName[]);
void write_to_file_external(word_list_block *block, char fileName[], symbol_table *table);
void initialize_new_file_name(file *file_, error_array *error, char name[]);

#endif
