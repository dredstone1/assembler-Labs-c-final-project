#ifndef MMN14_WORD_H
#define MMN14_WORD_H
#include "opcode.h"
#include "addressing_modes.h"
#include "operand_address.h"

typedef struct{
    opcode opcode;
    operand_address source_operand_address;
    operand_address destination_operand_address;
    addressing_mode addressing_mode;
} word;

#endif //MMN14_WORD_H
