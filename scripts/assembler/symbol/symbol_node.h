#ifndef MMN14_SYMBOL_NODE_H
#define MMN14_SYMBOL_NODE_H

#include "symbol.h"

typedef struct symbol_node{
    symbol symbol;
    struct symbol_node *next;
} symbol_node;

symbol_node *create_symbol_node(symbol symbol);

#endif
