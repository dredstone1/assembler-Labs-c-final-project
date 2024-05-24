#include "opcode.h"
#include <string.h>

const char *opcode_names[] = {
        "MOV",
        "CMP",
        "ADD",
        "SUB",
        "LEA",
        "CLR",
        "NOT",
        "INC",
        "DEC",
        "JMP",
        "BNE",
        "RED",
        "PRN",
        "JSR",
        "RTS",
        "STOP"
};

opcode get_opcode_from_string(const char* str) {
    for (int i = 0; i < sizeof(opcode_names) / sizeof(opcode_names[0]); i++) {
        if (strcmp(str, opcode_names[i]) == 0)
            return (opcode)i; // Cast to opcode enum
    }

    return -1; // Invalid opcode
}
