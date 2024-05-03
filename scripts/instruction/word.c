#include "word.h"
#include "../binary_translator.h"

int word_to_intBINARY(word word){
    int word_table[4];
    word_table[0] = word.opcode;
    word_table[1] = word.source_operand_address.address;
    word_table[2] = word.destination_operand_address.address;
    word_table[3] = word.addressing_mode;

    return array_of_int_to_binaryINT(word_table, 4);
}
