#include "opcode.h"
#include <string.h>

const char *opcode_names[] = {
        "mov",
        "cmp",
        "add",
        "sub",
        "lea",
        "clr",
        "not",
        "inc",
        "dec",
        "jmp",
        "bne",
        "red",
        "prn",
        "jsr",
        "rts",
        "stop"
};

opcode get_opcode_from_string(const char *str) {
    opcode code;
    for (code = MOV; code <= STOP; code++) {
        if (strcmp(str, opcode_names[code]) == 0) {
            return code;
        }
    }
    return -1;
}

int amount_of_variables_from_opcode(opcode code) {
     if (code < FIRST_GROUP_OPCODE)
        return 2;
     else if (code < SECOND_GROUP_OPCODE+FIRST_GROUP_OPCODE)
        return 1;
     else if (code < THIRD_GROUP_OPCODE+SECOND_GROUP_OPCODE+FIRST_GROUP_OPCODE)
        return 0;
     return -1;
}