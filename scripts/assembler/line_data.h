#ifndef MMN14_LINE_DATA_H
#define MMN14_LINE_DATA_H

#include "../data/line.h"
#include "symbol/symbol_node.h"
#include "opcode.h"
#include "directive.h"

typedef enum variable_type{
    IMMEDIATE,
    DIRECT,
    REGISTER_INDIRECT,
    REGISTER_DIRECT
} variable_type;

typedef struct line_directive{
    directive_type type;
    int variables[];
} line_directive;

typedef struct variable{
    int value;
    variable_type type;
    char symbol[MAX_SYMBOL_SIZE];
} variable;

typedef struct line_command{
    opcode opcode;
    variable variables[2];
} line_command;

typedef struct line_data{
    line_directive *directive;
    line_command *command;
} line_data;

void line_data_set(line_data *data, int offset, char line[]);

#endif //MMN14_LINE_DATA_H
