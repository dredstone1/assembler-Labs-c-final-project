#include "symbol_table.h"
#include <stdlib.h>

void add_symbol_to_symbol_table(symbol_table *table, symbol_node *node){
    if(table->head == NULL){
        table->head = node;
        return;
    }

    symbol_node *current = table->head;
    while(current->next != NULL){
        current = current->next;
    }

    current->next = node;

}

void add_symbol_to_end(symbol_table *table, symbol_node *node){
    if(table->head == NULL){
        table->head = node;
        return;
    }

    symbol_node *current = table->head;
    while(current->next != NULL){
        current = current->next;
    }

    current->next = node;
}
