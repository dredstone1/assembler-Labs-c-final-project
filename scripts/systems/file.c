#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include "../assembler/translation/utilities/octal_translator.h"
#include <string.h>
#include "error.h"

void free_line(line_node *node);

void read_file(file *file1, error *error) {
    char word[LINE_SIZE], word_temp[LINE_SIZE];
    int i = 0, line_number = 0;
    line_node *last_line;
    FILE *files;

    files = fopen(file1->filename, "r");
    if (files == NULL) {
        error->error_type = MEMORY_ALLOCATION_FAILED;
        return;
    }

    file1->number_of_rows = 0;
    last_line = create_line_node(NULL);
    if (last_line == NULL) {
        error->error_type = MEMORY_ALLOCATION_FAILED;
        return;
    }

    file1->first_line = last_line;

    while (fgets(word, sizeof(char)*LINE_SIZE, files) != NULL) {
        i=0;
        skip_spaces_and_tabs(&i, word);
        line_number++;
        if (word[i] == ';' || word[i] == '\n')
            continue;

        i=0;
        get_next_word(word_temp, &i, word, "\r\n", 2);
        word_temp[i]= '\0';
        if (file1->number_of_rows>0)
            last_line = last_line->next = create_line_node(NULL);

        if (last_line == NULL) {
            error->error_type = MEMORY_ALLOCATION_FAILED;
            return;
        }

        strcpy(last_line->line_text.content,word_temp);
        file1->number_of_rows++;
        last_line->line_number = line_number;
    }

    fclose(files);
}

void print_pos(int line, int column) {
    printf("%d, %d", line, column);
}

void free_file_lines(file *file1){
    free_line(file1->first_line);
}

void write_to_file_file(file file){
    char *file_name = strdup(file.filename), *dot = strrchr(file_name, '.');
    line_node *node = file.first_line;
    FILE *new_file;

    if (dot != NULL)
        strncpy(dot, ".am", 3);

    new_file = fopen(file_name, "w");

    while (node!=NULL){
        fprintf(new_file, "%s", node->line_text.content);
        if(node->next!=NULL)
            fprintf(new_file, "\n");

        node = node->next;
    }

    fclose(new_file);
}

void write_to_file_object(word_list_block *block, char fileName[]){
    word_node *node = block->head;
    char *file_name = strdup(fileName), *dot = strrchr(file_name, '.');
    int DC = 100;
    FILE *file;

    if (dot != NULL)
        strncpy(dot, ".ob", 3);

    file = fopen(file_name, "w");

    while (node!=NULL){
        fprintf(file, "%04d %05d", DC, int_to_octal(node->word));
        if(node->next!=NULL)
            fprintf(file, "\n");

        node = node->next;
        DC++;
    }

    fclose(file);
}

void write_to_file_external(word_list_block *block, char fileName[], symbol_table *table){
    word_node *node = block->head;
    char *file_name, *dot;
    FILE *file = NULL;
    int DC = 100;
    symbol *symbol;

    while (node!=NULL){
        if (node->symbol[0]!='\0') {
            symbol = get_symbol_address_from_symbol_name(table, node->symbol);

            if (symbol != NULL && symbol->type == EXTERNAL) {
                if (file==NULL){
                    file_name = strdup(fileName);
                    dot = strrchr(file_name, '.');
                    if (dot != NULL)
                        strncpy(dot, ".ext", 4);

                    file = fopen(file_name, "w");
                }
                fprintf(file, "%s  %d", node->symbol, DC);
                if (node->next != NULL)
                    fprintf(file, "\n");
            }
        }

        node = node->next;
        DC++;
    }

    fclose(file);
}

void write_to_file_entry(symbol_table *symbol_table, char fileName[]){
    symbol_node *node = symbol_table->head;
    char *file_name, *dot;
    symbol *symbol;
    FILE *file1 = NULL;

    while (node!=NULL){
        if (node->symbol.type == ENTRY_) {
            symbol = get_symbol_address_from_symbol_name(symbol_table, node->symbol.label);
            if (symbol != NULL) {
                if (file1 == NULL){
                    file_name = strdup(fileName);
                    dot = strrchr(file_name, '.');
                    if (dot != NULL)
                        strncpy(dot, ".ent", 4);

                    file1 = fopen(file_name, "w");
                }

                fprintf(file1, "%s  %d", symbol->label, symbol->address);
                if (node->next != NULL)
                    fprintf(file1, "\n");
            }
        }

        node = node->next;
    }

    fclose(file1);
}

void add_ending_to_file_name(char **fileName, error *error){
    int file_name_length = strlen(*fileName);
    *fileName = realloc(*fileName, sizeof(char) * (file_name_length + 4));
    if (*fileName == NULL) {
        error->error_type = MEMORY_ALLOCATION_FAILED;
        /*not enough memory error*/
        return;
    }
    strcpy(*fileName+file_name_length, ".as");
}
