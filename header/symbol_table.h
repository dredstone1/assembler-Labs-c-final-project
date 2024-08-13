#ifndef MMN14_SYMBOL_TABLE_H
#define MMN14_SYMBOL_TABLE_H

#include "utilities.h"
#include "error.h"


/**
 * @brief structure to store the symbol address.
 */
typedef struct symbol_address {
	int address; /* the address of the symbol */
	char symbol_name[MAX_SYMBOL_SIZE]; /* the name of the symbol */
	int line_number; /* the line number of the symbol declaration */
	int is_data_line; /* data flag */
	int external; /* external flag */
} symbol_address;


/**
 * @brief Checks if a string is a valid symbol name(at the start of the line).
 *
 * This function determines if the input string `symbol` represents a valid symbol name.
 * if the string does not end with a colon, it will return 2 (not a symbol).
 * else:
 * 		If the string if the string is a valid symbol name, it will return 1.
 * 		If the string is not a valid symbol name, it will return 0.
 *
 * @param symbol The string to be validated as a symbol name.
 * @return - 2: If the string is not a symbol name.
 * 		   - 1: If the string is a valid symbol name.
 *         - 0: If the string is not a valid symbol name.
 */
int is_valid_symbol(char *symbol);


/**
 * @brief Checks if a given string is a valid symbol name.
 *
 * A valid symbol name is a string that follows these rules:
 *  - It must start with a letter (a-z or A-Z).
 *  - It can contain letters and digits (0-9).
 *  - It cannot exceed a maximum length (defined by `MAX_SYMBOL_SIZE`).
 *
 * This function checks if the provided string `symbol` adheres to these rules.
 *
 * @param symbol The string to be validated as a symbol name.
 * @return 1 if the string is a valid symbol name, 0 otherwise.
 */
int is_valid_symbol_name(char *symbol);


/**
 * @brief Adds a new symbol to the symbol table.
 *
 * This function adds a new symbol to the given symbol table.
 * It relocates memory to accommodate the new symbol.
 * If the memory allocation fails, the function will return 0.
 * otherwise, it will fill the new symbol with the provided data.
 * and increment the size of the symbol table.
 * 
 *
 * @param symbols A pointer to the symbol table.
 * @param size A pointer to the current size of the table.
 * @param address The memory address associated with the symbol.
 * @param line_number The line number where the symbol is defined in the assembly code.
 * @param symbol_name The name of the symbol to be added.
 * @param is_data_line A flag indicating whether the symbol is defined in the data segment.
 * @param error A pointer to an `error` structure for error reporting.
 * @param external A flag indicating if the symbol is external. 
 * @return 1 if the symbol is added successfully, 0 otherwise.
 */
int add_symbol(symbol_address **symbols, int *size, int address, int line_number, char symbol_name[], int is_data_line,
			   error *error, int external);


/**
 * @brief Updates the addresses of symbols in the symbol table.
 *
 * This function iterates through the symbol table and updates
 * the addresses of symbols that have a matching data flag
 * as the provided `is_data_line` flag by adding the given value `by`.
 *
 * @param symbols The symbol table to update.
 * @param by The value to add to the address of each symbol containing the data flag.
 * @param size The number of symbols in the table.
 * @param is_data_line The data flag to match when updating the addresses.
 */
void update_table_by(symbol_address *symbols, int by, int size, int is_data_line);


/**
 * @brief Searches for a symbol in the symbol table by its name.
 *
 * This function iterates through the symbol table `symbols` to find
 * a symbol with a name matching the provided `symbol_name`,
 * if the symbol is found, the function returns the index of the symbol in the table.
 * 
 * If the symbol is not found, the function returns -1.
 *
 * @param symbol_name The name of the symbol to search for.
 * @param symbols The symbol table.
 * @param number_of_symbols The number of symbols in the table.
 * @return The index of the symbol in the table, if found, 
 *         otherwise -1 to indicate the symbol was not found. 
 */
int search_symbol_by_name(char symbol_name[], symbol_address *symbols, int number_of_symbols);

#endif
