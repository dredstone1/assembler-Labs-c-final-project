#include "opcode.h"
#include <string.h>
#include <stdio.h>
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
    opcode code = MOV;
    for (; code <= STOP; code++) {
        if (strcmp(str, opcode_names[code]) == 0) {
            return code;
        }
    }
/*
    printf("comparing %s to %s\n", str, opcode_names[code-1]);
*/
    return -1; // Or handle invalid opcodes differently
}



