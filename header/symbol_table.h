#ifndef MMN14_SYMBOL_TABLE_H
#define MMN14_SYMBOL_TABLE_H

#include "utilities.h"
#include "error.h"
/*
#include "line_data.h"
*/

#define MAX_SYMBOL_SIZE 31


typedef struct symbol_address {
	int address;
	char *symbol_name;
	int line_number;
	int is_data_line;
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



int add_symbol(symbol_address **symbols, int *size, int address, int line_number, char *symbol_name, int is_data_line, error_array *error);



int update_table_by(symbol_address *symbols, int by, int size, int is_data_line, error_array *error);


#endif
