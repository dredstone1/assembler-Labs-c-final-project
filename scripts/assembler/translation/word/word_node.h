#ifndef MMN14_WORD_NODE_H
#define MMN14_WORD_NODE_H

#include "word.h"

typedef struct word_node{
    word word;
    char symbol[MAX_SYMBOL_SIZE];
    struct word_node *next;
} word_node;

word_node* create_new_word_node(word word, int line_number, char *line, error_array *error);

#endif
