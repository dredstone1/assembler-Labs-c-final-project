#ifndef MMN14_FILE_H
#define MMN14_FILE_H

#include "../data/boolean.h"
#include "error.h"
#include "../data/line.h"
#include "../assembler/first_pass/words_block.h"

typedef struct file{
    line_node *first_line;
    int *number_of_rows;
    char *filename;
} file;

void read_file(file *file1, error *error);
void print_pos(int line, int column);
void free_file_lines(file *file1);
void write_to_file_file(file file);
void write_to_file_object(word_list_block *block, char fileName[]);

#endif //MMN14_FILE_H
