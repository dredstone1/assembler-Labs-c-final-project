#ifndef MMN14_LINE_DATA_H
#define MMN14_LINE_DATA_H

#include "symbol_table.h"
#include "consts.h"


#define MAX_LIST_SIZE 77 /*80 - 3 = MAX_LINE_SIZE - MIN_DIRECTIVE_TYPE_NAME*/

#define FIRST_GROUP_OPCODE 5
#define SECOND_GROUP_OPCODE 9
#define THIRD_GROUP_OPCODE 2

typedef struct var{
	char *var;
	int value;
	variable_type type;
} var;

typedef struct command_data{
	char *label;
    int opcode;
	var source;
	var destination;
} command_data;


/**
 * @brief Check if a string is a directive
 * 
 * @param str The string to check
 * @return int 1 if its a directive, 0 if its not a directive
 */
int is_directive(char *str);


/**
 * @brief Check if a given string is a valid string declaration
 * 
 * @param str The string to check
 * @return int 1 if its a valid string declaration, 0 if its not a valid string declaration
 */
int read_string(char **workable_line, char *line, instruction_data *instruction, error_array *error, int line_number);


/**
 * @brief read list of numbers from a given string
 * 
 * @param workable_line The string to read from
 * @param line The original line
 * @param error The error array
 * @param line_number The line number
 * @param instruction The instruction data
 * @return int 1 if success, 0 if fail
 */
int read_data(char **workable_line, char *line, instruction_data *instruction, error_array *error, int line_number);


/**
 * @brief read entry or extern symbol
 * 
 * @param workable_line The string to read from
 * @param line The original line
 * @param instruction The instruction data
 * @param error The error array
 * @param line_number The line number
 * @return int 1 if success, 0 if fail
 */
int read_extern_or_entry_symbol(char **workable_line, char *line, instruction_data *instruction, error_array *error, int line_number);


/**
 * @brief check what type of opcode is the given string
 * 
 * @param str The string to check
 * @return opcode The opcode of the string
 */
opcode get_opcode_from_string(const char *str);


/**
 * 
 */
int read_command_variables(char **workable_line, char *line, command_data *command, error_array *error, int line_number);

/**
 * 
 */
int amount_of_variables_from_opcode(opcode code);


#endif
