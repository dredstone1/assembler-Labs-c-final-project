#ifndef MMN14_ERROR_H
#define MMN14_ERROR_H

#include "../data/boolean.h"
#include "../data/line.h"

#define start_format_line_error " | "
#define amount_of_spaces_at_start 4


#define FILE_NAME_ERROR "Error in file: %s\n"

/*error messages*/
#define FILE_NOT_FOUND_MESSAGE "File not found"
#define MEMORY_ALLOCATION_FAILED_MESSAGE "Memory allocation failed"
#define UNDEFINED_TAG_NAME_MESSAGE "DIRECTIVE_TYPE_MISSING"
#define INVALID_OPCODE_MESSAGE "Invalid opcode"
#define SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE "Symbol cannot be defined as both external and entry"
#define INVALID_COMMA_MESSAGE "Invalid comma usage"
#define MISSING_COMMA_MESSAGE "Missing comma"
#define EXTRA_COMMA_MESSAGE "Extra comma"
#define MISSING_START_QUOTE_MESSAGE "Missing starting quote"
#define MISSING_ENDING_QUOTE_MESSAGE "Missing ending quote"
#define SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE "Symbol cannot be defined as both external and entry"
#define INVALID_COMMA_MESSAGE "Invalid comma usage"
#define MISSING_COMMA_MESSAGE "Missing comma"
#define EXTRA_COMMA_MESSAGE "Extra comma"
#define MISSING_START_QUOTE_MESSAGE "Missing starting quote"
#define MISSING_ENDING_QUOTE_MESSAGE "Missing ending quote"

/*editions*/
#define NOT_A_VALID_OPCODE "Not a valid opcode"
#define MISSING_QUOTE "\""
#define MISSING_COMMA_A ","

typedef enum error_type{
    FILE_NOT_FOUND,
    MEMORY_ALLOCATION_FAILED,
    UNDEFINED_TAG_NAME,
    INVALID_OPCODE,
    SYMBOL_IN_EXTERNAL_OR_ENTRY,
    INVALID_COMMA,
    MISSING_COMMA,
    EXTRA_COMMA,
    MISSING_START_QUOTE,
    MISSING_ENDING_QUOTE,
    DIRECTIVE_TYPE_MISSING,
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
    char file_name[LINE_SIZE];
    error_type type;
}error;

typedef struct error_array{
    error *errors;
    int size;
    importance importance;
}error_array;


void handel_error(error_array error_array);
void add_error(error_array *error_array, error_type error_type, int line_number, int start_place_in_line, int end_place_in_line, importance importance, char line[], char file_name[], int mark_offset);
void initialize_error(error_array *error_array);

#endif
