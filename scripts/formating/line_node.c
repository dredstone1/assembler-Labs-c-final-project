#include "line_node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_linked_list(line_node *head) {
    line_node *temp = head;

    while(temp != NULL) {
        printf("->> %s ", temp->line_text.content);
        temp = temp->next;
    }
}

line_node *create_line_node(line_node *next, line_data *line_data){
    line_node *node = (line_node *)malloc(sizeof(line_node));
    if (node == NULL)
        return NULL;

    node->line_data = line_data;
    node->next = next;
    return node;
}


line_node *copy_block_of_nodes(line_node **last_node, line_node *node) {
    line_node *new_node;

    if (node == NULL) {
        *last_node = NULL;
        return NULL;
    }

    new_node = create_line_node(NULL, node->line_data);
    if (new_node == NULL) {
        *last_node = NULL;
        return NULL;
    }

    /*Copy the line_text content*/
    strncpy(new_node->line_text.content, node->line_text.content, LINE_SIZE);

    /*Copy the line_number*/
    new_node->line_number = node->line_number;

    /*Recursively copy the rest of the nodes*/
    new_node->next = copy_block_of_nodes(last_node, node->next);

    /*If this is the last node in the block, set the last_node pointer*/
    if (new_node->next == NULL)
        *last_node = new_node;

    return new_node;
}

void add_data_object_to_lines(line_node *head) {
    line_node *temp = head;

    while(temp != NULL) {
        temp->line_data = (line_data *)malloc(sizeof(line_data));
        if (temp->line_data == NULL)
            return;

        temp = temp->next;
    }
}