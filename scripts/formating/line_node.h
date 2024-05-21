#ifndef MMN14_LINE_NODE_H
#define MMN14_LINE_NODE_H

#include "directive_line_type.h"
#include "operation_line_type.h"
#include "../data/tag.h"

#define LINE_SIZE 80

typedef struct line_text{
    char content[LINE_SIZE];
} line_text;

typedef struct line_data{
    tag tag;
    bool directive;
    int offset;
    directive_line directive_line;
    operation_line operation_line;
} line_data;

typedef struct line_node{
    struct line_node *next;
    line_data *line_data;
    line_text line_text;
    int line_number;
} line_node;

void print_linked_list(line_node *head);
line_node *create_line_node(line_node *next, line_data *line_data);
line_node *duplicate_lines_node(line_node *node);
void add_data_object_to_lines(line_node *head);
void add_node_to_end(line_node *old, line_node *new);
char* combine_line_nodes_text(line_node *node, int line_amount);
int get_line_node_length(line_node *node);
void offset_line_node_by(line_node *node, int offset);
void set_offset_line_node(line_node *node, int offset);
void offset_line_node_by_i(line_node *node);
void free_line(line_node *node);

#endif //MMN14_LINE_NODE_H
