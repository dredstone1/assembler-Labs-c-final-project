#ifndef MMN14_LINE_DATA_H
#define MMN14_LINE_DATA_H

#include "../data/line.h"
#include "symbol/symbol_node.h"
#include "opcode.h"

typedef enum line_directive_type{
    DATA,
    STRING,
    ENTRY,
    EXTERN
} line_directive_type;

typedef struct line_directive{
    line_directive_type type;
    int variables[];
} line_directive;

typedef struct line_command{
    opcode opcode;
    char variable_src[LINE_SIZE];
    char variable_tar[LINE_SIZE];
} line_command;

typedef struct line_data{
    line_directive *directive;
    line_command *command;
} line_data;

#endif //MMN14_LINE_DATA_H
