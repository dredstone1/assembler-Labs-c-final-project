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

symbol_table *create_symbol_table(error_array *error){
    symbol_table *table = (symbol_table *)malloc(sizeof(symbol_table));
    if (table == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
        return NULL;
    }

    table->head = NULL;
    table->tail = NULL;
    return table;
}

symbol* get_symbol_address_from_symbol_name(symbol_table *table, char label[MAX_SYMBOL_SIZE]){
    symbol_node *current_node = table->head;

    while (current_node != NULL) {
        if (strcmp(current_node->symbol.label, label) == 0)
            return &current_node->symbol;

        current_node = current_node->next;
    }
    return NULL;
}

void free_symbol_table(symbol_table *table){
    symbol_node *current_node = table->head;
    symbol_node *next_node;

    while (current_node != NULL) {
        next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
    free(table);
}