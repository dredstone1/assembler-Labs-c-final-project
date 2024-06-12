#ifndef MMN14_ERROR_H
#define MMN14_ERROR_H

#include "../data/boolean.h"

/*error messages*/
#define FILE_NOT_FOUND_MESSAGE "File not found: %S"
#define MEMORY_ALLOCATION_FAILED "Memory allocation failed"
#define UNDEFINED_TAG_NAME_MESSAGE "DIRECTIVE_TYPE_MISSING"



typedef enum error_type{
    NOTHING,
    FILE_NOT_FOUND,
    UNDEFINED_TAG_NAME,
    MEMORY_ALLOCATION_FAILED,
    DIRECTIVE_TYPE_MISSING,
    SYMBOL_IN_EXTERNAL_OR_ENTRY,
    INVALID_COMMA,
    MISSING_COMMA,
    EXTRA_COMMA,
}error_type;

typedef struct error {
    error_type error_type;
    bool temp;

    char **error_data;
}error;

void handel_error(error *error);

#endif
