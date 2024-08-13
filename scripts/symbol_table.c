#include "../header/symbol_table.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int is_valid_symbol_name(char *symbol) {
	/* check if the symbol is not too long and the first character is a letter,
	 * if not return 0 */
	if (!isalpha(*symbol) || strlen(symbol) > MAX_SYMBOL_SIZE) {
		return 0;
	}

	/* iterate over the symbol name and check if the characters are letters or digits
	 * when it reaches the end of the string, it will exit the loop
	 * when it reaches a character that is not a letter or a digit, it will exit the loop */
	while (*(++symbol) != '\0' && (isdigit(*symbol) || isalpha(*symbol)));

	/* if the symbol name contains only letters and digits, the loop will exit when it reaches the end of the string,
	 * else it will exit when it reaches a character that is not a letter or a digit.
	 * if the symbol name contains only letters and digits, 
	 * the character will be '\0' and it will return 1, else it will return 0 */
	return *symbol == '\0';
}


int is_valid_symbol(char *symbol) {
	/* check if the symbol last character is a colon, if not return 2(2 is not a symbol) */
	if (symbol[strlen(symbol) - 1] != ':') {
		return 2;
	}
	
	/* remove the colon from the symbol name */
	symbol[strlen(symbol) - 1] = '\0';
	
	/* check if the symbol is a valid symbol name, and return the result */
	return is_valid_symbol_name(symbol);
}


int add_symbol(symbol_address **symbols, int *size, int address, int line_number, char symbol_name[], int is_data_line,
			   error *error, int external) {
	/* allocate memory for the new symbol, then check if the allocation was successful, if not return 0 */
	*symbols = (symbol_address *) use_realloc(*symbols, (*size + 1) * sizeof(symbol_address), error);
	if (*symbols == NULL) {
		return 0;
	}
	
	/* fill the new symbol with the given data */
	(*symbols)[*size].address = address;
	strcpy((*symbols)[*size].symbol_name, symbol_name);
	(*symbols)[*size].line_number = line_number;
	(*symbols)[*size].is_data_line = is_data_line;
	(*symbols)[*size].external = external;
	
	/* increment the size of the symbol table */
	(*size)++;
	
	/* The symbol was added successfully, return 1 */
	return 1;
}


void update_table_by(symbol_address *symbols, int by, int size, int is_data_line) {
	int i;
	
	/* iterate over the symbols and update the address of the symbols that are flagged as the given data flag */
	for (i = 0; i < size; i++) {
		/* if the data flag of the symbol is equal to the given data flag, update the address of the symbol by the given value */
		if (symbols[i].is_data_line == is_data_line) {
			symbols[i].address += by;
		}
	}
}


int search_symbol_by_name(char symbol_name[], symbol_address *symbols, int number_of_symbols) {
	int i;
	
	/* iterate over the symbols and check if the symbol name matches the given symbol name
	 * if it does, return the index of the symbol in the array */
	for (i = 0; i < number_of_symbols; i++) {
		/*compare the symbol name with the current symbol name in the array */
		if (strcmp(symbol_name, symbols[i].symbol_name) == 0) {
			return i;
		}
	}

	/* if the symbol was not found, return -1 */
	return -1;
}