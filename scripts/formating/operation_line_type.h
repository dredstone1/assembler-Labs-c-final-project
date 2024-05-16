#ifndef MMN14_OPERATION_LINE_TYPE_H
#define MMN14_OPERATION_LINE_TYPE_H

#include "../data/boolean.h"

#define MAX_OPERATION_SIZE 4
#define DATA_OPERATION "mov "
#define STRING_OPERATION "cmp "
#define ENTRY_OPERATION "add "
#define EXTERN_OPERATION "sub "
#define LEA_OPERATION "lea "
#define CLR_OPERATION "clr "
#define NOT_OPERATION "not "
#define INC_OPERATION "inc "
#define DEC_OPERATION "dec "
#define JMP_OPERATION "jmp "
#define BNE_OPERATION "bne "
#define RED_OPERATION "red "
#define PRN_OPERATION "prn "
#define JSR_OPERATION "jsr "
#define RTS_OPERATION "rts "
#define STOP_OPERATION "stop "



typedef enum oparation_type {
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
}operation_type;


typedef struct operation_line {
    operation_type type;
} operation_line;

bool is_operation_type_is(char line[], int *offset, char compare[]);
#endif //MMN14_OPERATION_LINE_TYPE_H
