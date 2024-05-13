#ifndef MMN14_LINE_H
#define MMN14_LINE_H

#include "../data/tag.h"
#include "directive_line_type.h"

#define LINE_SIZE 80
#define LINE_JUMPER_SIZE 1

typedef struct line_text{
    char content[LINE_SIZE];
} line_text;

typedef struct line_data{
    tag tag;
    bool directive;
    int offset;
    directive_line directive_line;
} line_data;

typedef struct line_node{
    struct line_node *next;
    line_data *line_data;
    line_text line_text;
    int line_number;
} line_node;

void print_linked_list(line_node *head);
line_node *create_line_node(line_node *next, line_data *line_data);
line_node *copy_block_of_nodes(line_node **last_node, line_node *node);
void add_data_object_to_lines(line_node *head);
void set_direct_line_type(line_node *node);

#endif //MMN14_LINE_H
