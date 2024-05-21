#include "opcode.h"
#include "../binary_translator.h"

int opcode_to_binary(opcode code){
    if (code!=EMPTY)
        return intBINARY_to_binary(code);
    return EMPTY_OPCODE_ERROR_VALUE;
}
