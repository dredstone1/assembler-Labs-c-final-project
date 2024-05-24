#ifndef MMN14_SYMBOL_TABLE_H
#define MMN14_SYMBOL_TABLE_H

#include "symbol_node.h"

typedef struct symbol_table{
    symbol_node *head;
    symbol_node *tail;
    int amount_of_symbols;
} symbol_table;

#endif //MMN14_SYMBOL_TABLE_H
