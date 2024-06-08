#ifndef MMN14_SYMBOL_TABLE_H
#define MMN14_SYMBOL_TABLE_H

#include "symbol_node.h"
#include "../../data/boolean.h"


typedef struct symbol_table{
    symbol_node *head;
    symbol_node *tail;
    int amount_of_symbols;
} symbol_table;

void add_symbol_to_symbol_table(symbol_table *table, symbol_node *node);
symbol_table *create_symbol_table();
bool is_symbol_legal(symbol_table *table, char label[MAX_SYMBOL_SIZE]);
symbol* get_symbol_address_from_symbol_name(symbol_table *table, char label[MAX_SYMBOL_SIZE]);

#endif
