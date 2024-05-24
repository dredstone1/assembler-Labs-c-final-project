#ifndef MMN14_DIRECTIVE_LINE_TYPE_H
#define MMN14_DIRECTIVE_LINE_TYPE_H

#include "../data/boolean.h"

#define MAX_DIRECTIVE_SIZE 7
#define DATA_DIRECTIVE "data "
#define STRING_DIRECTIVE "string "
#define ENTRY_DIRECTIVE "entry "
#define EXTERN_DIRECTIVE "extern "

typedef enum directive_type {
    DATA,
    STRING,
    ENTRY,
    EXTERN
}directive_type;

typedef struct directive_line {
    directive_type type;
} directive_line;

bool is_directive_type_is(char line[], int *offset, char compare[]);

#endif //MMN14_DIRECTIVE_LINE_TYPE_H
