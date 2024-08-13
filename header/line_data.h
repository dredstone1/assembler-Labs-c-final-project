#ifndef MMN14_LINE_DATA_H
#define MMN14_LINE_DATA_H

#include "symbol_table.h"
#include "consts.h"

/* the group of amount of variables by the opcode */
#define FIRST_GROUP_OPCODE 5
#define SECOND_GROUP_OPCODE 9
#define THIRD_GROUP_OPCODE 2

/* the range of numbers that can be in the data */
#define DATA_LARGER_NUMBER 16384
#define DATA_SMALLER_NUMBER -16384

/* the range of numbers that can be in the command */
#define COMMAND_LARGER_NUMBER 2047
#define COMMAND_SMALLER_NUMBER -2047


/**
 * @brief structure to store the data of the variable.
 */
typedef struct var {
	char var[MAX_SYMBOL_SIZE]; /* the name of the variable */
	int value; /* the value of the variable */
	variable_type type; /* the type of the variable */
} var;


/**
 * @brief structure to store the data of the command.
 */
typedef struct command_data {
	int opcode; /* the opcode of the command */
	var source; /* the source variable of the command */
	var destination; /* the destination variable of the command */
} command_data;


/**
 * @brief structure to store the data of the directive.
 */
typedef struct directive_data {
	short *numbers; /* the data of the directive */
	int size; /* the size of the data */
	char args[MAX_SYMBOL_SIZE]; /* the symbol of the directive if it has one */
	directive_type type; /* the type of the directive */
} directive_data;


/**
 * @brief Check if a string is a directive
 * 
 * This function checks if a string is a directive by checking if it starts with a '.'.
 * 
 * @param str The string to check
 * @return int 1 if its a directive, 0 if its not a directive
 */
int is_directive(char *str);


/**
 * @brief Reads and processes a string literal within an assembly line.
 * 
 * This function extracts a string literal from the provided `workable_line`, 
 * validates its format (ensuring it's enclosed in double quotes), and 
 * stores the processed string in the `directive_data` structure.
 * 
 * it define the content of the string, as the content between the most left " and the most right ",
 * so if the string is "abc"def" it will define the content as abc"def.
 * if the string is "abc"def"ghi it will define the content as abc"def.
 * 
 * if the string is have only one quote or no quotes at all, it will print the appropriate error.
 *
 *
 * @param workable_line A pointer to a pointer to the current position within the line being processed.
 * @param line The original line of assembly code being processed.
 * @param directive A pointer to the `directive_data` structure to store the processed string.
 * @param error A pointer to an `error` structure to report any parsing errors.
 * @param line_number The line number of the current line being processed.
 * @param start_line_pointer A pointer to the beginning of the original line of code.
 *
 * @return 1 if the string is successfully read and processed, 0 otherwise.
 */
int read_string(char **workable_line, char *line, directive_data *directive, error *error, int line_number,
				char *start_line_pointer);


/**
 * @brief Reads and processes a comma-separated list of numbers within a .data directive.
 *
 * This function extracts a sequence of numbers from the `workable_line`, validates their 
 * format, and stores them in the provided `directive_data` structure. It enforces that 
 * numbers are separated by a single comma, are within a valid range, and handles errors 
 * such as missing or extra commas, invalid number formats, and exceeding the maximum 
 * allowed range for data values.
 * 
 * the function will handle any error that will occur during the reading of the data,
 * and will print the appropriate error.
 *
 * @param workable_line A pointer to a pointer to the current position within the line being processed.
 * @param line The original line of assembly code being processed.
 * @param directive A pointer to the `directive_data` structure to store the processed numbers.
 * @param error A pointer to an `error` structure to report any parsing errors.
 * @param line_number The line number of the current line being processed.
 * @param start_line_pointer A pointer to the beginning of the original line of code.
 *
 * @return 1 if the data sequence is successfully read and processed, 0 otherwise.
 */
int read_data(char **workable_line, char *line, directive_data *directive, error *error, int line_number,
			  char *start_line_pointer);


/**
 * @brief Reads and validates an external or entry symbol from an assembly directive.
 *
 * This function extracts a symbol name from the `workable_line`, validates 
 * its format, and stores it as argument in the `directive_data` structure.
 * It handles directives of type `.extern` or `.entry`.
 * reporting the appropriate error messages if the symbol is missing or invalid.
 *
 * @param workable_line A pointer to a pointer to the current position within the line being processed.
 * @param line The original line of assembly code being processed.
 * @param directive A pointer to the `directive_data` structure where the symbol will be stored.
 * @param error A pointer to an `error` structure to report any error.
 * @param line_number The line number of the current line being processed.
 * @return 1 if the symbol is successfully read and processed, 0 otherwise.
 */
int read_extern_or_entry_symbol(char **workable_line, char *line, directive_data *directive, error *error,
								int line_number);


/**
 * @brief Reads and parses variables (operands) for an assembly command. 
 *
 * This function extracts and validates the source and destination variables(depending on the command)
 * for an assembly command based on the opcode specified in the `command_data` 
 * structure. It handles commands with either one or two operands, ensuring 
 * they are properly formatted and reporting any errors.
 *
 * 
 * @param workable_line A pointer to a pointer to the current position within the line being processed.
 * @param line The original line of assembly code being processed.
 * @param command A pointer to the `command_data` structure, containing the opcode and where 
 *               the variables will be stored.
 * @param error A pointer to an `error` structure to report any parsing errors.
 * @param line_number The line number of the current line being processed.
 * @param start_workable_line A pointer to the beginning of the original line of code.
 * @return 1 if the variables are successfully read and processed, 0 otherwise.
 */
int read_command_variables(char **workable_line, char *line, command_data *command, error *error, int line_number,
						   char *start_workable_line);

/**
 * @brief Determines the number of operands required by a given opcode.
 *
 * This function takes an `opcode` value as input and returns the number of operands 
 * (variables) that the opcode expects. It determines this based on the opcode's group:
 * - 2:  For opcodes in the first group (typically two-operand instructions).
 * - 1:  For opcodes in the second group (typically one-operand instructions).
 * - 0:  For opcodes in the third group (typically zero-operand instructions).
 * - -1: If the provided opcode is not valid.
 *
 * @param code The opcode for which to determine the number of operands.
 * @return The number of operands required by the opcode:
 *          - 2:  For opcodes in the first group (typically two-operand instructions).
 *          - 1:  For opcodes in the second group (typically one-operand instructions).
 *          - 0:  For opcodes in the third group (typically zero-operand instructions).
 *          - -1: If the provided opcode is not valid.
 */
int amount_of_variables_from_opcode(opcode code);


#endif
