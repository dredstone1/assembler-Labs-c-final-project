#include "words_block.h"
#include <stdlib.h>

word_list_block* create_word_list_block(){
    word_list_block *block = (word_list_block*)malloc(sizeof(word_list_block));
    block->head = NULL;
    block->tail = NULL;
    return block;
}
