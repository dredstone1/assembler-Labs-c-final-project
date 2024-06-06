#ifndef MMN14_SYMBOL_NODE_H
#define MMN14_SYMBOL_NODE_H

#include "symbol.h"

typedef struct symbol_node{
    char label[MAX_SYMBOL_SIZE];
    int address;
    struct symbol_node *next;
} symbol_node;
symbol_node *create_symbol_node(char label[MAX_SYMBOL_SIZE], int address);
#endif //MMN14_SYMBOL_NODE_H
