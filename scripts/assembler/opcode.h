#ifndef MMN14_OPCODE_H
#define MMN14_OPCODE_H

#define OPCODE_COUNT 16
#define FIRST_GROUP_OPCODE 5
#define SECOND_GROUP_OPCODE 9
#define THIRD_GROUP_OPCODE 2

typedef enum opcode{
    MOV,
    CMP,
    ADD,
    SUB,
    LEA,
    CLR,
    NOT,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP
} opcode;

int amount_of_variables_from_opcode(opcode code);
opcode get_opcode_from_string(const char* str);

#endif
