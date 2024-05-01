#ifndef MMN14_WORD_H
#define MMN14_WORD_H

#include "opcode.h"
#include "addressing_modes.h"
#include "operand_address.h"

typedef struct word{
    opcode opcode;
    operand_address source_operand_address;
    operand_address destination_operand_address;
    addressing_mode addressing_mode;
} word;

int word_to_intBINARY(word word);

#endif //MMN14_WORD_H
