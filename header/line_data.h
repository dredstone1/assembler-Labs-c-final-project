#ifndef MMN14_LINE_DATA_H
#define MMN14_LINE_DATA_H

#include "symbol_table.h"
#include "consts.h"

#define FIRST_GROUP_OPCODE 5
#define SECOND_GROUP_OPCODE 9
#define THIRD_GROUP_OPCODE 2

typedef struct var {
	char var[MAX_SYMBOL_SIZE];
	int value;
	variable_type type;
} var;

typedef struct command_data {
	int opcode;
	var source;
	var destination;
} command_data;


typedef struct directive_data {
	short *numbers;
	int size;
	char args[MAX_SYMBOL_SIZE];
	directive_type type;
} directive_data;


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
int
read_string(char **workable_line, char *line, directive_data *directive, error *error, int line_number, char *start_line_pointer);


/**
 * @brief read list of numbers from a given string
 * 
 * @param workable_line The string to read from
 * @param line The original line
 * @param error The error array
 * @param line_number The line number
 * @param directive The directive data
 * @return int 1 if success, 0 if fail
 */
int read_data(char **workable_line, char *line, directive_data *directive, error *error, int line_number,
			  char *start_line_pointer);


/**
 * @brief read entry or extern symbol
 * 
 * @param workable_line The string to read from
 * @param line The original line
 * @param directive The directive data
 * @param error The error array
 * @param line_number The line number
 * @return int 1 if success, 0 if fail
 */
int
read_extern_or_entry_symbol(char **workable_line, char *line, directive_data *directive, error *error, int line_number);


/**
 * 
 */
int read_command_variables(char **workable_line, char *line, command_data *command, error *error, int line_number,
						   char *start_workable_line);

/**
 * 
 */
int amount_of_variables_from_opcode(opcode code);


#endif
