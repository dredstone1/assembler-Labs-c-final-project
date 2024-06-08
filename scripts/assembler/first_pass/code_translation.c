#include "code_translation.h"
#include <stdio.h>
#include <stdlib.h>
#include "words_block.h"
#include "../../octal_translator.h"

void format_line(char line[LINE_SIZE], word_list_block *word_block, symbol_table *table, int IC);
void first_pass(file *file1, symbol_table *table, word_list_block *file_code_block);
void create_files(word_list_block *file_code_block, symbol_table *table, char file_name[]);

void translate_code(file *file1){
    word_list_block *file_code_block = create_new_word_list_block();
    word_node *current_word;
    symbol_table *table = create_symbol_table();
    first_pass(file1, table, file_code_block);

    add_symbols_to_code_block(file_code_block, table);
    current_word = file_code_block->head;
    while (current_word != NULL) {
        printf("%d\n", int_to_octal(current_word->word));
        current_word = current_word->next;
    }

    create_files(file_code_block, table, file1->filename);
}

void create_files(word_list_block *file_code_block, symbol_table *table, char file_name[]){
    write_to_file_object(file_code_block, file_name);
}

void first_pass(file *file1, symbol_table *table, word_list_block *file_code_block){
    line_node *current_line = file1->first_line;
    word_list_block *current_line_word_block;
    int IC = 100;

    while (current_line != NULL) {
        current_line_word_block = create_new_word_list_block();
        format_line(current_line->line_text.content, current_line_word_block, table, IC);
        if (current_line_word_block->head != NULL) {
            IC += current_line_word_block->size;
            combine_word_list_blocks(file_code_block, current_line_word_block, current_line->line_text.content);
        }
        current_line = current_line->next;
    }
}

void format_line(char line[LINE_SIZE], word_list_block *current_line_word_block, symbol_table *table, int IC) {
    int offset = 0;
    symbol *symbol;
    line_data data;
    data.directive = NULL;
    data.command = NULL;
    symbol = get_symbol(line, &offset);

    line_data_set(&data, offset, line, symbol);
    line_data_to_word_list_block(current_line_word_block, &data);

    if (symbol->label[0] != '\0') {
        if (is_symbol_legal(table, symbol->label)==FALSE) {
            // error symbol already exists
            return;
        }
        symbol->address = IC;
        add_symbol_to_symbol_table(table, create_symbol_node(*symbol));
    }
    free(symbol);
}
