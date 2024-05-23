#include "symbol_node.h"
#include <stdlib.h>

symbol_node *create_symbol_node(char label[31], int address, int is_external, int is_data, int is_instruction) {
    symbol_node *node = (symbol_node *) malloc(sizeof(symbol_node));
    node->label = label;
    node->address = address;
    node->is_external = is_external;
    node->is_data = is_data;
    node->is_instruction = is_instruction;
    node->next = NULL;
    return node;
}