#ifndef MMN14_OPCODE_H
#define MMN14_OPCODE_H

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



opcode get_opcode_from_string(const char* str);

#endif //MMN14_OPCODE_H
