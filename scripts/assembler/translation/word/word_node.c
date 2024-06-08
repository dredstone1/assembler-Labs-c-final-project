#include "word_node.h"
#include <stdlib.h>

word_node* create_new_word_node(word word){
    word_node *node = (word_node *)malloc(sizeof(word_node));
    if (node == NULL) {
        /*out of memory error*/
        return NULL;
    }
    node->symbol[0] = '\0';
    node->word = word;
    node->next = NULL;
    return node;
}
