#include "word.h"

void set_opcode_into_word(word *word, opcode op_code){
    (*word) |= op_code<<opcode_bit_shift;
}

void insert_operand_type_into_word(word *word, operand op, variable_type type){
    insert_value_into_word(word, (1<<type)<<(operand_bit_shift+operand_bit_size*op));
}

void set_ARE_into_word(word *word, ARE are){
    insert_value_into_word(word, 1<<are);
}

void insert_operand_into_word(word *word, int value){
    insert_value_into_word(word, value<<operand_bit_shift);
}

void insert_value_into_word(word *word, int value){
    (*word) |= value & 0x7FFF;
}
