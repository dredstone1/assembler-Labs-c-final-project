#include "../header/symbol_table.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int is_valid_symbol_name(char *symbol) {
	if (!isalpha(*symbol) || strlen(symbol) > MAX_SYMBOL_SIZE) {
		return 0;
	}

	while (*(++symbol) != '\0' && (isdigit(*symbol) || isalpha(*symbol)));

	if (*symbol == '\0') {
		return 1;
	}

	return 0;
}


int is_valid_symbol(char *symbol) {
	int status;
	if (symbol[strlen(symbol) - 1] != ':') {
		return 2;
	}

	symbol[strlen(symbol) - 1] = '\0';
	status = is_valid_symbol_name(symbol);

	return status;
}


int add_symbol(symbol_address **symbols, int *size, int address, int line_number, char symbol_name[], int is_data_line,
			   error *error, int external) {
	*symbols = (symbol_address *) use_realloc(*symbols, (*size + 1) * sizeof(symbol_address), error);
	if (*symbols == NULL) {
		print_system_error(MEMORY_ALLOCATION_FAILED_MESSAGE);
		error->importance = CRITICAL;
		return 0;
	}

	(*symbols)[(*size)].address = address;
	strcpy((*symbols)[(*size)].symbol_name, symbol_name);
	(*symbols)[(*size)].line_number = line_number;
	(*symbols)[(*size)].is_data_line = is_data_line;
	(*symbols)[(*size)].external = external;
	(*size)++;
	return 1;
}


int update_table_by(symbol_address *symbols, int by, int size, int is_data_line, error *error) {
	int i;
	for (i = 0; i < size; i++) {
		if (symbols[i].is_data_line == is_data_line) {
			symbols[i].address += by;
		}
	}

	return 1;
}


int search_symbol_by_name(char symbol_name[], symbol_address *symbols, int number_of_symbols) {
	int i;
	for (i = 0; i < number_of_symbols; i++) {
		if (strcmp(symbol_name, symbols[i].symbol_name) == 0) {
			return i;
		}
	}

	return -1;
}