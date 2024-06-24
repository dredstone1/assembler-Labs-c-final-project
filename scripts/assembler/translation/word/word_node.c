#include "word_node.h"
#include <stdlib.h>

word_node* create_new_word_node(word word, int line_number, char *line, error_array *error){
    word_node *node = (word_node *)malloc(sizeof(word_node));
    if (node == NULL) {
        add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
        return NULL;
    }
    node->symbol[0] = '\0';
	node->line_number = line_number;
	node->line = line;
    node->word = word;
    node->next = NULL;
    return node;
}
