#include "first_pass.h"
#include <stdio.h>
#include <stdlib.h>
#include "words_block.h"
#include "../symbol/symbol.h"
#include "../line_data.h"
#include "../symbol/symbol_table.h"

void format_line(char line[LINE_SIZE], word_list_block *word_block, symbol_table *table);

void first_pass(file *file1){
    line_node *current_line = file1->first_line;
    word_list_block *current_line_word_block, file_code_block;
    symbol_table table;


    while (current_line != NULL) {
        current_line_word_block = create_new_word_list_block();
        format_line(current_line->line_text.content, current_line_word_block, &table);

        current_line = current_line->next;
    }
}

void format_line(char line[LINE_SIZE], word_list_block *current_line_word_block, symbol_table *table){
    int offset=0;
    char *symbol;
    line_data data;
    data.directive = NULL;
    data.command = NULL;
    symbol = get_symbol(line, &offset);

    line_data_set(&data, offset, line);

    line_data_to_word_list_block(current_line_word_block, &data);

    if (symbol != NULL) {
/*
        add_symbol_to_symbol_table(table, create_symbol_node(symbol, data->address));
*/
    }
    free(symbol);
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
