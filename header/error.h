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
#define INVALID_OPCODE_MESSAGE "Invalid opcode:"
#define SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE "Symbol cannot be defined as both external and entry:"
#define INVALID_COMMA_MESSAGE "Invalid comma usage:"
#define MISSING_COMMA_MESSAGE "Missing comma:"
#define EXTRA_COMMA_MESSAGE "Extra comma:"
#define MISSING_START_QUOTE_MESSAGE "Missing starting quote:"
#define MISSING_ENDING_QUOTE_MESSAGE "Missing ending quote:"
#define MISSING_ENDING_QUOTE_N_START_QUOTE_MESSAGE "Missing ending quote and starting quote:"
#define DIRECTIVE_TYPE_MISSING_MESSAGE "directive type missing:"
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
#define MISSING_SYMBOL "Missing symbol:"
#define MISSING_NUMBER "Missing number:"

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
#define DIRECTIVE_TYPE_MISSING_DESCRIPTION "missing directive type, you should have a directive after the point"
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
#define DIRECTIVE_NAME_AS_MARCO_NAME_DESCRIPTION "macro name cannot be an directive name"
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
#define MISSING_SYMBOL_IN_EXTERN_DESCRIPTION "missing symbol in extern directive."
#define MISSING_SYMBOL_IN_ENTRY_DESCRIPTION "missing symbol in entry directive."
#define MISSING_SYMBOL_DESCRIPTION "missing symbol."
#define MISSING_NUMBER_DESCRIPTION "missing number after the comma."
#define MISSING_NUMBER_AFTER_NUMBER_STARTER_DESCRIPTION "missing number after the #."



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


/**
 * @brief Prints a system error message.
 *
 * This function prints a given system error message to the standard output.
 *
 * @param error_massage The system error message to be printed.
 */
void print_system_error(char error_massage[]);


/**
 * @brief Prints a general error message with quoting the error location.
 *
 * This function prints an error message along with the relevant line of code and its line number.
 * It highlights the specific portion of the line where the error occurred and provides a description of the error.
 * This function is used when the error location is to be quoted in to the description.
 *
 * @param error_massage The main error message to be printed.
 * @param error_description A detailed description of the error.
 * @param line The line of code where the error occurred.
 * @param line_number The line number of the code where the error occurred.
 * @param start_place_in_line The starting position of the error in the line.
 * @param end_place_in_line The ending position of the error in the line.
 * @param mark_offset1 The first offset for marking the error.
 * @param mark_offset2 The second offset for marking the error.
 * @param error A pointer to the error structure that keeps track of error counts and file names.
 */
void print_general_error(char error_massage[], char error_description[], char line[], int line_number, int start_place_in_line, int end_place_in_line, int mark_offset1, int mark_offset2, error *error);


/**
 * @brief Prints a general error message without quoting the error location.
 *
 * This function prints an error message along with the relevant line of code and its line number.
 * It highlights the specific portion of the line where the error occurred and provides a description of the error.
 * This function is used when the error location is not to be quoted in to the description.
 *
 * @param error_massage The main error message to be printed.
 * @param error_description A detailed description of the error.
 * @param line The line of code where the error occurred.
 * @param line_number The line number of the code where the error occurred.
 * @param start_place_in_line The starting position of the error in the line.
 * @param end_place_in_line The ending position of the error in the line.
 * @param mark_offset1 The first offset for marking the error.
 * @param mark_offset2 The second offset for marking the error.
 * @param error A pointer to the error structure that keeps track of error counts and file names.
 */
void print_general_error_no_quoting(char error_massage[], char error_description[], char line[], int line_number, int start_place_in_line, int end_place_in_line, int mark_offset1, int mark_offset2, error *error);


/**
 * @brief Prints a detailed error message spanning two lines of code.
 *
 * This function prints an error message along with the relevant lines of code and their respective line numbers.
 * It highlights the specific portions of the lines where the error occurred and provides a description of the error.
 *
 * @param error_massage The main error message to be printed.
 * @param error_description A detailed description of the error.
 * @param line1 The first line of code where the error occurred.
 * @param line2 The second line of code where the error occurred.
 * @param line1_number The line number of the first line of code.
 * @param line2_number The line number of the second line of code.
 * @param start_place_in_line1 The starting position of the error in the first line.
 * @param end_place_in_line1 The ending position of the error in the first line.
 * @param start_place_in_line2 The starting position of the error in the second line.
 * @param end_place_in_line2 The ending position of the error in the second line.
 * @param error A pointer to the error structure that keeps track of error counts and file names.
 */
void print_two_line_error(char error_massage[], char error_description[], char line1[], char line2[], int line1_number, int line2_number, int start_place_in_line1, int end_place_in_line1, int start_place_in_line2, int end_place_in_line2, error *error);


/**
 * @brief Prints a separator line.
 * 
 * This function prints a separator line of dashes('-'). The length of the line is determined by the `MAX_LINE_LENGTH` constant.
 * If `from_beginning` is 0, it prints a line of length `MAX_LINE_LENGTH`.
 * otherwise, it prints a line of length `MAX_LINE_LENGTH + SIDE_LENGTH`.
 * 
 * @param from_beginning Indicates whether to print the separator from the beginning (1) or not (0).
 */
void print_separator(int from_beginning);


/**
 * @brief Prints an error message for illegal command variables.
 * 
 * This function prints an error message indicating that a variable in a command is not of a legal type.
 * If this is the first error for the file, it prints the file name and a separator. For subsequent errors, it prints only a separator.
 * It highlights the illegal variable in the line and provides a detailed error description, including the legal variable types.
 * 
 * @param variable The variable that is not of a legal type.
 * @param line The line containing the illegal variable.
 * @param line_number The line number where the error occurred.
 * @param error A pointer to an error structure containing error details.
 * @param variable_type_valid A string indicating the valid variable types.
 * @param usage Indicates whether the variable is used as a source (0) or destination (1).
 * @param code The opcode of the command.
 */
void print_command_not_legal(char variable[], char line[], int line_number, error *error,const char variable_type_valid[], int usage, opcode code);


/**
 * @brief Prints an error message when a label is conflicting with a macro name.
 * 
 * This function prints an error message indicating that a label is conflicting with a macro name. It includes the file name and line number in the message.
 * If this is the first error for the file, it prints the file name and a separator. For subsequent errors, it prints only a separator.
 * It highlights the conflicting macro name in the line and provides a detailed error description.
 * 
 * @param line The line containing the conflict.
 * @param line_number The line number where the conflict occurred.
 * @param macro_line_number The line number where the macro is defined.
 * @param error A pointer to an error structure containing error details.
 * @param macro The conflicting macro name.
 */
void print_macro_b_label_same(char line[], int line_number, int macro_line_number, error *error, char macro[]);


/**
 * @brief Prints an error message for lines that are too long at the end of processing.
 * 
 * This function prints an error message indicating that a line is too long. It is typically called at the end of processing.
 * 
 * This function is the second part of the error message. The first part is printed by print_start_line_too_long_error().
 * between the two parts, the line that is too long is printed.
 */
void print_end_line_too_long_error();


/**
 * @brief Prints an error message for lines that are too long at the start of processing.
 * 
 * This function prints an error message indicating that a line is too long. It includes the file name and line number in the message.
 * If this is the first error for the file, it prints the file name and a separator. For subsequent errors, it prints only a separator.
 * 
 * This function is the first part of this error message. The second part is printed by print_end_line_too_long_error().
 * between the two parts, the line that is too long is printed.
 * 
 * @param line_number The line number where the error occurred.
 * @param error A pointer to an error structure containing error details.
 */
void print_start_line_too_long_error(int line_number, error *error);


/**
 * @brief Sets the file extension to a specified ending.
 * 
 * This function changes the file extension of the given file name to the specified ending.
 * It finds the position of the current extension and replaces it with the new ending.
 * 
 * @param fileName The name of the file to modify.
 * @param ending The new file extension to set.
 */
void set_ending_to_file_name(char fileName[], char ending[]);
#endif
