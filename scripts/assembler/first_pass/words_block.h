#ifndef MMN14_WORDS_BLOCK_H
#define MMN14_WORDS_BLOCK_H

#include "word_node.h"
#include "../line_data.h"

typedef struct word_list_block{
    word_node *head;
    word_node *tail;
} word_list_block;

void line_data_to_word_list_block(word_list_block *block, line_data *data);
word_list_block* create_new_word_list_block();

#endif
