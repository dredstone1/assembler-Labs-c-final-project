#include "../header/line_node.h"
#include "../header/utilities.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

line_node *create_line_node(line_node *next){
    line_node *node = (line_node *)use_malloc(sizeof(line_node));
    if (node == NULL)
        return NULL;

    node->next = next;
    return node;
}

line_node* duplicate_line_node(line_node *node) {
    line_node *new_node;
    if (node == NULL)
        return NULL;

    new_node = create_line_node(NULL);
    if (new_node == NULL)
        return NULL;

    strncpy(new_node->line_text.content, node->line_text.content, LINE_SIZE);
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
    free(node);
}