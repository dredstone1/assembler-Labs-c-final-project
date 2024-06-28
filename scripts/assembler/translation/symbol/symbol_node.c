#include "symbol_node.h"
#include <stdlib.h>
#include <string.h>

symbol_node *create_symbol_node(symbol symbol) {
    symbol_node *node = (symbol_node *) malloc(sizeof(symbol_node));
    strncpy(node->symbol.label, symbol.label, MAX_SYMBOL_SIZE);
    node->symbol.address = symbol.address;
    node->symbol.type = symbol.type;

    node->next = NULL;

    return node;
}