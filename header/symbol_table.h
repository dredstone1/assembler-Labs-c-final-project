#ifndef MMN14_SYMBOL_TABLE_H
#define MMN14_SYMBOL_TABLE_H

#include "utilities.h"
#include "error.h"


typedef struct symbol_address {
	int address;
	char symbol_name[MAX_SYMBOL_SIZE];
	int line_number;
	int is_data_line;
	int external;
} symbol_address;


/**
 * @brief Check if a symbol is valid
 * 
 * @param symbol The symbol to check
 * @return int 1 if its a valid symbol, 0 if its not a symbol, 2 if the symbol is invalid 
 */
int is_valid_symbol(char *symbol);

/**
 * @brief Check if a symbol name is valid
 * 
 * @param symbol The symbol to check
 * @return int 1 if its a valid symbol, 2 if the symbol is invalid 
 */
int is_valid_symbol_name(char *symbol);


int add_symbol(symbol_address **symbols, int *size, int address, int line_number, char symbol_name[], int is_data_line,
			   error *error, int external);


int update_table_by(symbol_address *symbols, int by, int size, int is_data_line);


/**
 * @brief Search for a symbol by its name
 * 
 * @param symbol_name The name of the symbol to search for
 * @param symbols The array of symbols to search in
 * @param number_of_symbols The number of symbols in the array
 * @return int The index of the symbol in the array, -1 if the symbol was not found
 */
int search_symbol_by_name(char symbol_name[], symbol_address *symbols, int number_of_symbols);


void handle_duplication(symbol_address *symbol_table, int label_amount, error *error);

#endif
