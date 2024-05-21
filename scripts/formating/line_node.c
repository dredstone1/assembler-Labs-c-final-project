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

line_data* duplicate_line_data(line_data *data) {
    line_data *new_data;

    if (data == NULL)
        return NULL;

    new_data = (line_data *)malloc(sizeof(line_data));
    if (new_data == NULL)
        return NULL;

    new_data->directive = data->directive;
    new_data->offset = data->offset;
    new_data->tag = data->tag;
    new_data->directive_line = data->directive_line;
    new_data->operation_line = data->operation_line;

    return new_data;
}

line_node* duplicate_line_node(line_node *node) {
    line_node *new_node;
    if (node == NULL)
        return NULL;

    new_node = create_line_node(NULL, duplicate_line_data(node->line_data));
    if (new_node == NULL)
        return NULL;

    /*Copy the line_text content*/
    strncpy(new_node->line_text.content, node->line_text.content, LINE_SIZE);
    /*Copy the line_number*/
    new_node->line_number = node->line_number;

    return new_node;
}

line_node *duplicate_lines_node(line_node *node) {
    line_node *next_node, *head;

    head = next_node = duplicate_line_node(node);
    if (next_node == NULL)
        return NULL;

    next_node = next_node;
    while (node != NULL) {
        next_node->next = duplicate_line_node(node);

        node = node->next;
        next_node = next_node->next;
    }
    return head;
}

void add_node_to_end(line_node *old, line_node *new) {
    line_node *temp = old;
    while(temp->next != NULL)
        temp = temp->next;

    temp->next = new;
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

int get_line_node_length(line_node *node){
    int i=0;
    while (node!=NULL){
        node = node->next;
        i++;
    }

    return i;
}

void offset_line_node_by(line_node *node, int offset) {
    while (node != NULL) {
        node->line_number +=offset;
        node = node->next;
    }
}

void offset_line_node_by_i(line_node *node) {
    int i=0;
    while (node != NULL) {
        node->line_number +=i;
        node = node->next;
        i++;
    }
}

void set_offset_line_node(line_node *node, int offset) {
    while (node != NULL) {
        node->line_number = offset;
        node = node->next;
    }
}

void free_line(line_node *node){
    if (node == NULL)
        return;

    free_line(node->next);

    free(node->line_data);
    free(node);
}