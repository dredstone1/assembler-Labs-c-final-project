#include "word.h"
#include "../binary_translator.h"

int word_to_intBINARY(word){
    return array_of_int_to_binaryINT(word.opcode, word.source_operand_address, word.destination_operand_address, word.addressing_mode)
}
