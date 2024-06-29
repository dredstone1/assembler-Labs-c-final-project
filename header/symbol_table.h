#ifndef MMN14_SYMBOL_TABLE_H
#define MMN14_SYMBOL_TABLE_H

#include "utilities.h"
#include "error.h"

#define MAX_SYMBOL_SIZE 31

typedef enum symbol_type{
	START = 0,
	EXTERNAL = 3,
	ENTRY_ = 2
} symbol_type;

typedef struct symbol{
	symbol_type type;
	char label[MAX_SYMBOL_SIZE];
	int address;
} symbol;

typedef struct symbol_table{
	symbol *symbols;
	int size;
} symbol_table;

symbol* get_symbol(char line[], int *offset, error_array *error);
void add_symbol(symbol_table *table, char *label, error_array *error, int address, symbol_type type);
void initialize_symbol_table(symbol_table *symbol_table_);
symbol* get_symbol_address_from_symbol_name(symbol_table *table, char label[MAX_SYMBOL_SIZE]);

#endif
