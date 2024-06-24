#ifndef MMN14_WORDS_BLOCK_H
#define MMN14_WORDS_BLOCK_H

#include "word_node.h"
#include "../symbol/symbol_table.h"

typedef struct word_list_block{
    word_node *head;
    word_node *tail;
    int size;
} word_list_block;

void line_data_to_word_list_block(word_list_block *block, line_data *data, error_array *error, int line_number, char line[]);
word_list_block* create_new_word_list_block(error_array *error);
void combine_word_list_blocks(word_list_block *block1, word_list_block *block2);
void add_symbols_to_code_block(word_list_block *block, symbol_table *symbol_table, error_array *error);
void free_word_list_block(word_list_block *block);

#endif
