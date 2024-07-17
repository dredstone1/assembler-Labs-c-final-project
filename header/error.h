#ifndef MMN14_ERROR_H
#define MMN14_ERROR_H

/*
#include "utilities.h"
*/
#include "line_node.h"

#define start_format_line_error " | "
#define amount_of_spaces_at_start 4


#define FILE_NAME_ERROR "Error in file: %s\n"

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
#define DIRECTIVE_TYPE_MISSING_MESSAGE "Directive type missing:"
#define SYMBOL_NOT_FOUND_MESSAGE "Symbol not found:"
#define INVALID_VARIABLE_TYPE_MESSAGE "Invalid variable type:"


typedef enum error_message_stage {
    PRINT_GENERAL_ERROR_TYPE,
    PRINT_ERROR_LINE,
    PRINT_ERROR_MARK,
	PRINT_ERROR_MESSAGE_CONCLUSION,
    END_OF_ERROR,
	FOR_EVERY_ERROR
}error_message_stage;


#define ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE " is "
#define ERROR_DESCRIPTION_MESSAGE_START_PATTERN_DOUBLE " and "

/*descriptions*/
#define INVALID_OPCODE_DESCRIPTION "not a valid opcode."
#define SYMBOL_IN_EXTERNAL_OR_ENTRY_DESCRIPTION "not a valid symbol , symbol cannot be defined as both external and entry."
#define INVALID_COMMA_DESCRIPTION "not a valid comma usage."
#define MISSING_COMMA_DESCRIPTION "missing comma, you should have a comma between the two operands."
#define EXTRA_COMMA_DESCRIPTION "having extra comma, you should have only one comma between the two operands."
#define MISSING_START_QUOTE_DESCRIPTION "missing a quote at the beginning of the string."
#define MISSING_ENDING_QUOTE_DESCRIPTION "missing a quote at the end of the string."
#define MISSING_ENDING_QUOTE_N_START_QUOTE_DESCRIPTION "missing a quote at the end of the string and a quote at the beginning of the string."
#define DIRECTIVE_TYPE_MISSING_DESCRIPTION "missing directive type, you should have a directive type before the variables."
#define SYMBOL_NOT_FOUND_DESCRIPTION "not initialized, you need to initialize the symbol"
#define INVALID_VARIABLE_TYPE_DESCRIPTION "not a valid variable type."

typedef enum error_type{
    FILE_NOT_FOUND,
    MEMORY_ALLOCATION_FAILED,
    UNDEFINED_SYMBOL_NAME,
    INVALID_OPCODE,
    SYMBOL_IN_EXTERNAL_OR_ENTRY,
    INVALID_COMMA,
    MISSING_COMMA,
    EXTRA_COMMA,
    MISSING_START_QUOTE,
    MISSING_ENDING_QUOTE,
    MISSING_ENDING_QUOTE_N_START_QUOTE,
	INVALID_DIRECTIVE_TYPE,
	SYMBOL_NOT_FOUND,
	INVALID_VARIABLE_TYPE,
	INVALID_SYMBOL_NAME,
	INVALID_NUMBER,
	MISSING_STRING,
    NOTHING
}error_type;

typedef enum importance{
    NO_ERROR,
    WARNING,
    CRITICAL
} importance;


typedef struct error {
    int line_number;
    int start_place_in_line;
    int end_place_in_line;
    int mark_offset;
    char line[LINE_SIZE];
    error_type type;
}error;

typedef struct error_array{
    error *errors;
    int size;
    importance importance;
}error_array;


void handel_error(error_array error_array, char *file_name);
void add_error(error_array *error_array, error_type error_type, int line_number, int start_place_in_line,
			   int end_place_in_line, importance importance, char line[], int mark_offset);
void initialize_error(error_array *error_array);

#endif
