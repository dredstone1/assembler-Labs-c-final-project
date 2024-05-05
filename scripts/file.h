#ifndef MMN14_FILE_H
#define MMN14_FILE_H

#include "boolean.h"
#include "error.h"
#include "formating/line_type.h"
#include "tag.h"

#define LINE_SIZE 80
#define LINE_JUMPER_SIZE 1

typedef struct line{
    char content[LINE_SIZE];
    tag tag;
} line;


typedef struct pos{
    int line;
    int column;
} pos;

typedef struct file{
    line *line;
    int number_of_rows;
    char *filename;
    bool ext;
    bool ent;
    pos *pos;
} file;

void read_file(file *file1, error *error);
void print_pos(pos pos);
#endif //MMN14_FILE_H
