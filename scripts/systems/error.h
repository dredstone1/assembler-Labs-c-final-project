#ifndef MMN14_ERROR_H
#define MMN14_ERROR_H

#include "../data/boolean.h"

/*error messages*/
#define FILE_NOT_FOUND_MESSAGE "File not found: %S"
#define MEMORY_ALLOCATION_FAILED_MESSAGE "Memory allocation failed"
#define UNDEFINED_TAG_NAME_MESSAGE "DIRECTIVE_TYPE_MISSING"
#define INVALID_OPCODE_MESSAGE "Invalid opcode: %s"
#define SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE "Symbol cannot be defined as both external and entry: %s"
#define INVALID_COMMA_MESSAGE "Invalid comma usage"
#define MISSING_COMMA_MESSAGE "Missing comma"
#define EXTRA_COMMA_MESSAGE "Extra comma"
#define MISSING_START_QUOTE_MESSAGE "Missing starting quote"
#define MISSING_ENDING_QUOTE_MESSAGE "Missing ending quote"
#define SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE "Symbol cannot be defined as both external and entry: %s"
#define INVALID_COMMA_MESSAGE "Invalid comma usage"
#define MISSING_COMMA_MESSAGE "Missing comma"
#define EXTRA_COMMA_MESSAGE "Extra comma"
#define MISSING_START_QUOTE_MESSAGE "Missing starting quote"
#define MISSING_ENDING_QUOTE_MESSAGE "Missing ending quote"

typedef enum error_type{
    NOTHING,
    FILE_NOT_FOUND,
    UNDEFINED_TAG_NAME,
    MEMORY_ALLOCATION_FAILED,
    INVALID_OPCODE,
    DIRECTIVE_TYPE_MISSING,
    SYMBOL_IN_EXTERNAL_OR_ENTRY,
    INVALID_COMMA,
    MISSING_COMMA,
    EXTRA_COMMA,
    MISSING_START_QUOTE,
    MISSING_ENDING_QUOTE,
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
    error_type type;
}error;

typedef struct error_array{
    error *errors;
    int size;
    importance importance;
}error_array;


void handel_error(error_array error_array);
void add_error(error_array *error_array, error_type error_type, int line_number, int start_place_in_line, int end_place_in_line, importance importance);
error create_error(error_type type, int line_number, int start_place_in_line, int end_place_in_line);
void error_out_of_memory();
void initialize_error(error_array *error_array);
void free_error(error_array error_array);

#endif
