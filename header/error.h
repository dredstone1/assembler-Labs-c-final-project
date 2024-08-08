#ifndef MMN14_ERROR_H
#define MMN14_ERROR_H


#include "../header/consts.h"

#define start_format_line_error " | "
#define amount_of_spaces_at_start 4
#define SIDE_LENGTH 7


#define FILE_NAME_ERROR "Error in file \"%s\":\n"

/*error messages*/
#define FILE_NOT_FOUND_MESSAGE "File not found:"
#define MEMORY_ALLOCATION_FAILED_MESSAGE "Memory allocation failed"
#define UNDEFINED_TAG_NAME_MESSAGE "DIRECTIVE_TYPE_MISSING:"
#define INVALID_OPCODE_MESSAGE "Invalid opcode:"
#define SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE "Symbol cannot be defined as both external and entry:"
#define INVALID_COMMA_MESSAGE "Invalid comma usage:"
#define MISSING_COMMA_MESSAGE "Missing comma:"
#define EXTRA_COMMA_MESSAGE "Extra comma:"
#define MISSING_START_QUOTE_MESSAGE "Missing starting quote:"
#define MISSING_ENDING_QUOTE_MESSAGE "Missing ending quote:"
#define MISSING_ENDING_QUOTE_N_START_QUOTE_MESSAGE "Missing ending quote and starting quote:"
#define INSTRUCTION_TYPE_MISSING_MESSAGE "instruction type missing:"
#define SYMBOL_NOT_FOUND_MESSAGE "Symbol not found:"
#define MISSING_PARAMETER_MESSAGE_ONE "Missing parameter:"
#define MISSING_PARAMETER_MESSAGE_TWO "Missing parameter:"
#define INVALID_SYMBOL_NAME_MESSAGE "Invalid symbol name:"
#define INVALID_DIRECTIVE_TYPE_MESSAGE "Invalid directive type:"
#define DUPLICATE_SYMBOL_MESSAGE "Duplicate symbol:"
#define INVALID_VARIABLE_TYPE_MESSAGE "Invalid variable type:"
#define EXTRA_TEXT_MESSAGE "Text after end of line:"
#define EXTRA_PARAMETER_MESSAGE "Extra parameter:"
#define ILLEGAL_MACRO_NAME_MESSAGE "illegal macro name:"
#define SYMBOL_CAN_BE_SAME_AS_MACRO_NAME "symbol cannot be the same as a macro name:"
#define INVALID_REGISTER_NAME_MESSAGE "Invalid register name:"
#define LINE_TOO_LONG_MESSAGE "Line is too long:"
#define UNDEFINED_OPCODE_MESSAGE "Undefined opcode:"
#define INVALID_NUMBER_MESSAGE "Invalid number:"

#define ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE " is "

/*descriptions*/
#define INVALID_OPCODE_DESCRIPTION "not a valid opcode."
#define SYMBOL_IN_EXTERNAL_OR_ENTRY_DESCRIPTION "not a valid symbol, symbol cannot be defined as both external and entry."
#define INVALID_COMMA_DESCRIPTION "not a valid comma usage."
#define MISSING_COMMA_DESCRIPTION "missing comma, you should have a comma between the two operands."
#define EXTRA_COMMA_DESCRIPTION "There is an extra comma in the operand list. You should have only one comma between the two operands."
#define INVALID_COMMA_AFTER_COMMAND_TYPE_DESCRIPTION "not a valid comma usage, you should not have a comma after the command type."
#define EXTRA_COMMA_DATA_DESCRIPTION "There is an extra comma in the data list. You should have only one comma between numbers."
#define MISSING_START_QUOTE_DESCRIPTION "missing a quote at the beginning of the string."
#define MISSING_ENDING_QUOTE_DESCRIPTION "missing a quote at the end of the string."
#define MISSING_ENDING_QUOTE_N_START_QUOTE_DESCRIPTION "missing a quote at the end of the string and a quote at the beginning of the string."
#define INSTRUCTION_TYPE_MISSING_DESCRIPTION "missing instruction type, you should have a directive after the point"
#define SYMBOL_NOT_FOUND_DESCRIPTION "not initialized, you need to initialize the symbol"
#define INVALID_VARIABLE_TYPE_DESCRIPTION "not a valid variable type."
#define MISSING_PARAMETER_DESCRIPTION_ONE "missing a parameter, you need one parameters for this command."
#define MISSING_PARAMETER_DESCRIPTION_TWO "missing a parameter, you need two parameters for this command."
#define INVALID_SYMBOL_NAME_DESCRIPTION "not a valid symbol name."
#define INVALID_DIRECTIVE_TYPE_DESCRIPTION "not a valid directive type."
#define SYMBOL_ALREADY_EXISTS_DESCRIPTION "already declared in line:"
#define INVALID_VARIABLE_TYPE_SOURCE_DESCRIPTION "not a valid source variable type, "
#define INVALID_VARIABLE_TYPE_DESTINATION_DESCRIPTION "not a valid destination variable type, "
#define EXTRA_TEXT_DESCRIPTION "there is text after the end of the line."
#define EXTRA_PARAMETER_DESCRIPTION "extra parameter, you should have only one parameter for this command."
#define OPCODE_NAME_AS_MARCO_NAME_DESCRIPTION "macro name cannot be an opcode name"
#define INSTRUCTION_NAME_AS_MARCO_NAME_DESCRIPTION "macro name cannot be an instruction name"
#define REGISTER_NAME_AS_MARCO_NAME_DESCRIPTION "macro name cannot be a register name"
#define MACRO_NAME_IS_TOO_LONG_DESCRIPTION "macro name is too long, maximum length is 31 characters."
#define SYMBOL_CAN_BE_SAME_AS_MACRO_NAME_DESCRIPTION "not a valid symbol name, symbol cannot be the same as a macro name."
#define INVALID_REGISTER_NAME_DESCRIPTION "not a valid register name."
#define LINE_TOO_LONG_DESCRIPTION "line is too long, maximum length is 80 characters."
#define UNDEFINED_OPCODE_DESCRIPTION "no instruction found."
#define INVALID_NUMBER_DESCRIPTION "not a valid number."
#define SYMBOL_IS_TOO_LONG_DESCRIPTION "symbol is too long, maximum length is 31 characters."
#define MACRO_NAME_CANNOT_BE_SAVED_WORD_DESCRIPTION "macro name cannot be a saved word."
#define SYMBOL_ALREADY_EXISTS_AS_MACRO_DESCRIPTION "symbol already exists as a macro, in file: %s, line:%d"

#define LEGAL_VARIABLE_TYPES_MESSAGE "support "

typedef enum importance{
    NO_ERROR,
    WARNING,
    CRITICAL
} importance;

typedef struct error{
	int error_single_file_count;
	importance importance;
	char *file_name;
	int error_count;
}error;


void print_system_error(char error_massage[]);
void print_general_error(char error_massage[], char error_description[], char line[], int line_number, int start_place_in_line, int end_place_in_line, int mark_offset1, int mark_offset2, error *error);
void print_general_error_no_quoting(char error_massage[], char error_description[], char line[], int line_number, int start_place_in_line, int end_place_in_line, int mark_offset1, int mark_offset2, error *error);
void print_two_line_error(char error_massage[], char error_description[], char line1[], char line2[], int line1_number, int line2_number, int start_place_in_line1, int end_place_in_line1, int start_place_in_line2, int end_place_in_line2, error *error);
void print_separator(int from_beginning);
void print_command_not_legal(char variable[], char line[], int line_number, error *error,const char variable_type_valid[], int usage, opcode code);
void print_macro_b_label_same(char line[], int line_number, int macro_line_number, error *error, char macro[]);
void print_end_line_too_long_error();
void print_start_line_too_long_error(int line_number, error *error);


/**
 * @brief Sets the ending to a file name.
 *
 * @param fileName The file name to set the ending for.
 * @param ending The ending to set.
 */
void set_ending_to_file_name(char fileName[], char ending[]);
#endif
