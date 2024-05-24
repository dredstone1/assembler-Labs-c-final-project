#ifndef MMN14_WORDS_BLOCK_H
#define MMN14_WORDS_BLOCK_H

#include "word_node.h"

typedef struct word_list_block{
    word_node *head;
    word_node *tail;
} word_list_block;

word_list_block* create_word_list_block();

#endif //MMN14_WORDS_BLOCK_H
