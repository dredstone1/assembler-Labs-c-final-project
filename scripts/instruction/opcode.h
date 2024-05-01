#ifndef MMN14_OPCODE_H
#define MMN14_OPCODE_H

#define EMPTY_OPCODE_ERROR_VALUE 2
typedef enum {
    MO,
    CM,
    AD,
    SU,
    NO,
    CL,
    LE,
    IN,
    DE,
    JM,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP,
    EMPTY
} opcode;

int opcode_to_int(opcode code);

#endif //MMN14_OPCODE_H
