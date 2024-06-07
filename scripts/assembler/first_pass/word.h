#ifndef MMN14_WORD_H
#define MMN14_WORD_H

#include "../opcode.h"
#include "../line_data.h"

#define opcode_bit_shift 11
#define operand_bit_shift 3
#define operand_bit_size 4

typedef enum operand{
    DESTINATION,
    SOURCE
} operand;

typedef enum ARE{
    E,
    R,
    A
} ARE;


typedef short word;

void set_opcode_into_word(word *word, opcode op_code);
void insert_operand_type_into_word(word *word, operand op, variable_type type);
void set_ARE_into_word(word *word, ARE are);
#endif
