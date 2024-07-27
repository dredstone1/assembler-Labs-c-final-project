#include "../header/symbol_table.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int is_valid_symbol_name(char *symbol){
	if (!isalpha(*symbol) || strlen(symbol) > MAX_SYMBOL_SIZE)
		return 2;
	
	while (*(++symbol) != '\0' && (isdigit(*symbol) || isalpha(*symbol)));
	if (*symbol == ':')
		return 1;
	
	return 2;
}

int is_valid_symbol(char *symbol){
	int status;
	if (symbol[strlen(symbol)-1] != ':')
		return 0;
	
	symbol[strlen(symbol)] = '\0';
	status = is_valid_symbol_name(symbol);
	
	symbol[strlen(symbol)-1] = ':';

	return status;
}


int add_symbol(symbol_address **symbols, int *size, int address, int line_number, char symbol_name[], int is_data_line, error_array *error){
	*symbols = (symbol_address*) use_realloc(*symbols, (*size + 1) * sizeof(symbol_address), error);
	if (*symbols == NULL){
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0, 0);
		return 0;
	}

	(*symbols)[(*size)].address = address;
	(*symbols)[(*size)].symbol_name = (char*)use_malloc((strlen(symbol_name) + 1)*sizeof(char), error);
	strcpy((*symbols)[(*size)].symbol_name, symbol_name);
	(*symbols)[(*size)].line_number = line_number;
	(*symbols)[(*size)].is_data_line = is_data_line;
	(*size)++;
	return 1;
}

int update_table_by(symbol_address *symbols, int by, int size, int is_data_line, error_array *error){
	int i;
	for (i = 0; i < size; i++)
		if (symbols[i].is_data_line == is_data_line)
			symbols[i].address += by;

	return 1;
}


void free_symbol_table(symbol_address *symbols, int size){
	int i;
	for (i = 0; i < size; i++)
		free(symbols[i].symbol_name);
	free(symbols);
}
