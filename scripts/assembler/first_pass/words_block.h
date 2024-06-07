#ifndef MMN14_WORDS_BLOCK_H
#define MMN14_WORDS_BLOCK_H

#include "word_node.h"
#include "../line_data.h"

typedef struct word_list_block{
    word_node *head;
    word_node *tail;
    int size;
} word_list_block;

void line_data_to_word_list_block(word_list_block *block, line_data *data);
word_list_block* create_new_word_list_block();
void combine_word_list_blocks(word_list_block *block1, word_list_block *block2, char line[LINE_SIZE]);

#endif
