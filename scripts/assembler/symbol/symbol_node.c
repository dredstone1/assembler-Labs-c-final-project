#include "symbol_node.h"
#include <stdlib.h>
#include <string.h>

symbol_node *create_symbol_node(char label[MAX_SYMBOL_SIZE], int address) {
    symbol_node *node = (symbol_node *) malloc(sizeof(symbol_node));
    strncpy(node->label, label, MAX_SYMBOL_SIZE);
    node->address = address;
    node->next = NULL;

    return node;
}