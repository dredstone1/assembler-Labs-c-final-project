#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

void add_symbol_to_symbol_table(symbol_table *table, symbol_node *node){
    if(table->head == NULL){
        table->head = table->tail = node;
        node->next = NULL; /* Ensuring the new tail's next is NULL */
    } else {
        table->tail->next = node; /* Link the new node as the next of the current tail */
        table->tail = node; /* Update the tail pointer to the new node */
        node->next = NULL; /* Ensuring the new tail's next is NULL */
    }
}

symbol_table *create_symbol_table(){
    symbol_table *table = (symbol_table *)malloc(sizeof(symbol_table));
    table->head = NULL;
    table->tail = NULL;
    table->amount_of_symbols = 0;
    return table;
}

bool is_symbol_legal(symbol_table *table, char label[MAX_SYMBOL_SIZE]){
    symbol_node *current_node = table->head;

    while (current_node != NULL) {
        if (current_node->symbol.type != ENTRY_ && strcmp(current_node->symbol.label, label) == 0)
            return FALSE;

        current_node = current_node->next;
    }
    return TRUE;
}