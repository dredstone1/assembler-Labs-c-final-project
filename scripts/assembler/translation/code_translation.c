#include "code_translation.h"
#include <stdio.h>
#include <stdlib.h>

void format_line(char line[LINE_SIZE], word_list_block *word_block, symbol_table *table, int IC, error *error);
void first_pass(file *file1, symbol_table *table, word_list_block *file_code_block, error *error);
void create_files(word_list_block *file_code_block, symbol_table *table, char file_name[]);

void translate_code(file *file1, error *error){
    word_list_block *file_code_block = create_new_word_list_block(error);
    symbol_table *table = create_symbol_table(error);
    first_pass(file1, table, file_code_block, error);

    add_symbols_to_code_block(file_code_block, table);
    create_files(file_code_block, table, file1->filename);

    free_symbol_table(table);
    free_word_list_block(file_code_block);
}

void create_files(word_list_block *file_code_block, symbol_table *table, char file_name[]){
    write_to_file_object(file_code_block, file_name);
    write_to_file_entry(table, file_name);
    write_to_file_external(file_code_block, file_name, table);
}

void first_pass(file *file1, symbol_table *table, word_list_block *file_code_block, error *error){
    line_node *current_line = file1->first_line;
    word_list_block *current_line_word_block;
    int IC = 100;

    while (current_line != NULL) {
        current_line_word_block = create_new_word_list_block(error);
        format_line(current_line->line_text.content, current_line_word_block, table, IC, error);
        if (current_line_word_block->head != NULL) {
            IC += current_line_word_block->size;
            combine_word_list_blocks(file_code_block, current_line_word_block);
        }
        current_line = current_line->next;
    }
}

void format_line(char line[LINE_SIZE], word_list_block *current_line_word_block, symbol_table *table, int IC, error *error) {
    int offset = 0;
    symbol *symbol;
    line_data data;
    data.directive = NULL;
    data.command = NULL;
    symbol = get_symbol(line, &offset, error);
    if (symbol == NULL)
        return;

    line_data_set(&data, offset, line, symbol, error);
    line_data_to_word_list_block(current_line_word_block, &data);

    if (symbol->label[0] != '\0') {
        if (data.directive != NULL && data.directive->type == EXTERNAL)
            symbol->address = 0;
        else
            symbol->address = IC;
        add_symbol_to_symbol_table(table, create_symbol_node(*symbol));
    }
    free(symbol);
}
