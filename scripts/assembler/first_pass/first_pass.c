#include "first_pass.h"
#include <stdio.h>
#include <stdlib.h>
#include "words_block.h"
#include "../symbol/symbol.h"
#include "../line_data.h"

void format_line(char line[LINE_SIZE], word_list_block *word_block);

void first_pass(file *file1){
    line_node *current_line = file1->first_line;
    word_list_block *words_block, *current_word_block;

    word_node *current_word;

    while (current_line != NULL) {
        current_word_block = create_word_list_block();
        format_line(current_line->line_text.content, current_word_block);

        current_line = current_line->next;
    }
}

void format_line(char line[LINE_SIZE], word_list_block *word_block){
    int offset=0;
    char *symbol;
    line_data *data = (line_data*) malloc(sizeof(line_data));

    symbol = get_symbol(line, &offset);

    line_data_set(data, offset, line);


    if (symbol != NULL) {
        /*add_symbol_to_symbol_table(word_block, symbol);*/
    }
    free(symbol);
    free(data);
}


















/*


void set_line_type(line_node *node){
    int i;

    node->line_data->directive = FALSE;

    for (i = node->line_data->offset; i < MAX_TAG_SIZE; i++) {
        if (node->line_text.content[i] == ' ' || node->line_text.content[i] == '\t')
            continue;
        else if (node->line_text.content[i] =='.') {
            node->line_data->directive = TRUE;
            node->line_data->offset = i+1;
        }

        return;
    }
}
*/
