#ifndef MMN14_LINE_H
#define MMN14_LINE_H

#include "../data/tag.h"

#define LINE_SIZE 80
#define LINE_JUMPER_SIZE 1

typedef struct line_text{
    char content[LINE_SIZE];
} line_text;

typedef struct line_data{
    tag tag;
} line_data;

typedef struct line{
    line_data *line_data;
    line_text line_text;
    int line_number;
} line;

typedef struct line_node{
    line *line;
    struct line_node *next;

} line_node;
line_node *create_line_node(line_node *next, line_data *line_data);
#endif //MMN14_LINE_H
