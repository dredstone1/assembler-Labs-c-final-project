#ifndef MMN14_FILE_H
#define MMN14_FILE_H

#include "../data/boolean.h"
#include "error.h"
#include "../data/pos.h"
#include "../formating/macro.h"
#include "../formating/line.h"

typedef struct file{
    line_node *first_line;
    int number_of_rows;
    char *filename;
    bool ext;
    bool ent;
    pos pos;
} file;

void read_file(file *file1, error *error);
void print_pos(pos pos);
#endif //MMN14_FILE_H
