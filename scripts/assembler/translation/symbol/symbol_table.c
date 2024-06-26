#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>


int symbol_name(char line[], int *offset, char word[]) {
	int i=0;

	get_next_word_n_skip(word, &i, line, " :\t \0", 4);

	if(line[i] != ':')
		return 0;

	*offset += i+1;
	return 1;
}

void add_symbol(symbol_table *table, symbol symbol){
	symbol *symbol_temp;
	symbol_temp = (symbol)realloc(table->, sizeof(error) * (error_array->size+1));
	if (error_array_temp != NULL) {
		error_array->errors = error_array_temp;
		error_array->size++;
	}
	else
		error_type = MEMORY_ALLOCATION_FAILED;

	error_array->importance = importance;
	error_array->errors[error_array->size-1].line_number = line_number;
	memcpy(error_array->errors[error_array->size-1].line,line, strlen(line));
	error_array->errors[error_array->size-1].start_place_in_line = start_place_in_line;
	error_array->errors[error_array->size-1].mark_offset = mark_offset;
	error_array->errors[error_array->size-1].end_place_in_line = end_place_in_line;
	error_array->errors[error_array->size-1].type = error_type;
	
}



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
        if (current_node->symbol.type != ENTRY_ && strcmp(current_node->symbol.label, label) == 0)
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