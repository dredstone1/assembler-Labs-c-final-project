#include "symbol_table.h"
#include <stdlib.h>


void add_symbol(symbol_table *table, symbol_node *node){
    if(table->head == NULL){
        table->head = node;
        table->tail = node;
    } else {
        table->tail->next = node;
        table->tail = node;
    }
}
