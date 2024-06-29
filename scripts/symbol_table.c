#include "../header/symbol_table.h"
#include <stdlib.h>
#include <string.h>


void add_symbol(symbol_table *table, char label[], error_array *error, int address, symbol_type type){
	table->size++;
	table->symbols = (symbol *)realloc(table->symbols, sizeof(symbol) * (table->size));
	if (table->symbols == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
		return;
	}

	table->symbols[table->size-1].type = type;

	strcpy(table->symbols[table->size-1].label, label);

	table->symbols[table->size-1].address = address;
}

void initialize_symbol_table(symbol_table *symbol_table_){
	symbol_table_->size = 0;
	symbol_table_->symbols = NULL;
}

symbol* get_symbol(char line[], int *offset, error_array *error){
	int i=0;
	char word[LINE_SIZE];
	symbol *symbol_obj = (symbol*)malloc(sizeof(symbol));
	
	if (symbol_obj == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
		return NULL;
	}

	symbol_obj->label[0] = '\0';
	get_next_word_n_skip(word, &i, line, " :\t \0", 4);
	if(line[i] != ':')
		return symbol_obj;

	strcpy(symbol_obj->label, word);
	symbol_obj->type = START;

	*offset += i+1;
	return symbol_obj;
}

symbol* get_symbol_address_from_symbol_name(symbol_table *table, char label[MAX_SYMBOL_SIZE]){
	int i;
	for (i = 0; i < table->size; i++) {
		if (table->symbols[i].type != ENTRY_ && strcmp(table->symbols[i].label, label) == 0)
			return &table->symbols[i];
	}
	
	return NULL;
}
