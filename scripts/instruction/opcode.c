#include "opcode.h"
#include "../binary_translator.h"

int opcode_to_binary(opcode code){
    if (code!=EMPTY)
        return int_to_binary(code);
    return EMPTY;
}
