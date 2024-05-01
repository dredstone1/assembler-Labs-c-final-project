#ifndef MMN14_OPCODE_H
#define MMN14_OPCODE_H

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

int opcode_to_binary(opcode code);

#endif //MMN14_OPCODE_H
