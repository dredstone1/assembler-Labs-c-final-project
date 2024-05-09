#include "line.h"
#include <stdlib.h>
#include <stdio.h>

line_node *create_line_node(line_node *next, line_data *line_data){
    line_node *node = (line_node *)malloc(sizeof(line_node));
    if (node == NULL)
        return NULL;

    if (line_data != NULL)
        node->line_data = line_data;

    node->next = next;
    return node;
}
