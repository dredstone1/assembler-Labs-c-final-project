#ifndef MMN14_LINE_DATA_H
#define MMN14_LINE_DATA_H

#include "../../../data/line.h"
#include "opcode.h"
#include "directive.h"
#include "../symbol/symbol.h"

#define MAX_LIST_SIZE 77 /*80 - 3 = MAX_LINE_SIZE - MIN_DIRECTIVE_TYPE_NAME*/

typedef enum variable_type{
    IMMEDIATE,
    DIRECT,
    REGISTER_INDIRECT,
    REGISTER_DIRECT,
    NONE= -1
} variable_type;

typedef struct variable{
    int value;
    variable_type type;
    char symbol[MAX_SYMBOL_SIZE];
} variable;

typedef struct line_directive{
    directive_type type;
    int variables[MAX_LIST_SIZE];
    int amount_of_variables;
} line_directive;

typedef struct line_command{
    opcode opcode;
    variable variables[2];
} line_command;

typedef struct line_data{
    line_directive *directive;
    line_command *command;
} line_data;

void line_data_set(line_data *data, int offset, char line[], symbol symbol[], error_array *error, int line_number, char file_name[]);

#endif
