#include "line.h"
#include <stdlib.h>
#include <stdio.h>
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

void set_direct_line_type(line_node *node){
    if(is_directive_type_is(node->line_text.content, &node->line_data->offset, DATA_DIRECTIVE))
        node->line_data->directive_line.type = DATA;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, STRING_DIRECTIVE))
        node->line_data->directive_line.type = STRING;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, ENTRY_DIRECTIVE))
        node->line_data->directive_line.type = ENTRY;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, EXTERN_DIRECTIVE))
        node->line_data->directive_line.type = EXTERN;
    else{

        /*error undefined direct line type*/
    }
}

void set_operation_line_type(line_node *node){
    if(is_directive_type_is(node->line_text.content, &node->line_data->offset, DATA_OPERATION))
        node->line_data->directive_line.type = MOV;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, STRING_OPERATION))
        node->line_data->directive_line.type = CMP;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, ENTRY_OPERATION))
        node->line_data->directive_line.type = ADD;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, EXTERN_OPERATION))
        node->line_data->directive_line.type = SUB;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, LEA_OPERATION))
        node->line_data->directive_line.type = LEA;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, CLR_OPERATION))
        node->line_data->directive_line.type = CLR;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, NOT_OPERATION))
        node->line_data->directive_line.type = NOT;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, INC_OPERATION))
        node->line_data->directive_line.type = INC;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, DEC_OPERATION))
        node->line_data->directive_line.type = DEC;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, JMP_OPERATION))
        node->line_data->directive_line.type = JMP;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, BNE_OPERATION))
        node->line_data->directive_line.type = BNE;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, RED_OPERATION))
        node->line_data->directive_line.type = RED;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, PRN_OPERATION))
        node->line_data->directive_line.type = PRN;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, JSR_OPERATION))
        node->line_data->directive_line.type = JSR;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, RTS_OPERATION))
        node->line_data->directive_line.type = RTS;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, STOP_OPERATION))
        node->line_data->directive_line.type = STOP;
    else{
        /*error undefined operation type*/
    }
}
