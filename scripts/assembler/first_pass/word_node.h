#ifndef MMN14_WORD_NODE_H
#define MMN14_WORD_NODE_H

#include "word.h"

typedef struct word_node{
    word word;
    struct word_node *next;
} word_node;

word_node* create_new_word_node(word word);

#endif
