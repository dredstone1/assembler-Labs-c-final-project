#include "../header/file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void free_line(line_node *node);

void read_file(file *file_, error_array *error) {
    char word[LINE_SIZE], word_temp[LINE_SIZE];
    int i = 0, line_number = 0;
    line_node *last_line;
    FILE *files;
	set_ending_to_file_name(file_->filename, "as");
    files = fopen(file_->filename, "r");
    if (files == NULL) {
		add_error(error, FILE_NOT_FOUND, 0, 0, 0, WARNING, "", 0);
        return;
    }

	file_->number_of_rows = 0;
    last_line = create_line_node(NULL);
    if (last_line == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
        return;
    }

	file_->first_line = last_line;

    while (fgets(word, sizeof(char)*LINE_SIZE, files) != NULL) {
        i=0;
		skip_spaces_and_tabs_with_offset(&i, word);
        line_number++;
        if (word[i] == ';' || word[i] == '\n')
            continue;

        i=0;
/*        get_next_word(word_temp, &i, word, "\r\n", 2);*/
        word_temp[i] = '\0';
        if ((file_->number_of_rows)>0)
            last_line = last_line->next = create_line_node(NULL);

        if (last_line == NULL) {
			add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
            return;
        }

        strcpy(last_line->line_text.content,word_temp);
		file_->number_of_rows++;
        last_line->line_number = line_number;
    }

    fclose(files);
}

void free_file_lines(file *file1){
    free_line(file1->first_line);
}

void write_to_file(file file){
    line_node *node = file.first_line;
    FILE *new_file;

	set_ending_to_file_name(file.filename, "am");

    new_file = fopen(file.filename, "w");

    while (node!=NULL){
        fprintf(new_file, "%s", node->line_text.content);
        if(node->next!=NULL)
            fprintf(new_file, "\n");

        node = node->next;
    }
	
	set_ending_to_file_name(file.filename, "as");
    fclose(new_file);
}

/*void write_to_file_object( *block, char fileName[], int IC, int DC){
    word_node *node = block->head;
    int current_word_line = 100;
    FILE *file;
	
	set_ending_to_file_name(fileName, "ob");

    file = fopen(fileName, "w");
	fprintf(file, "%5d %d\n", DC, IC);
    while (node!=NULL){
        fprintf(file, "%04d %05d", DC, int_to_octal(node->word));
        if(node->next!=NULL)
            fprintf(file, "\n");

        node = node->next;
		current_word_line++;
    }
	
	set_ending_to_file_name(fileName, "as");
    fclose(file);
}

void write_to_file_external(word_list_block *block, char fileName[], symbol_table *table){
    word_node *node = block->head;
    FILE *file = NULL;
    int DC = 100;
    symbol *symbol;

	set_ending_to_file_name(fileName, "ext");

    while (node!=NULL){
        if (node->symbol[0]!='\0') {
            symbol = get_symbol_address_from_symbol_name(table, node->symbol);

            if (symbol != NULL && symbol->type == EXTERNAL) {
                if (file==NULL){
                    file = fopen(fileName, "w");
                }
                fprintf(file, "%s  %d", node->symbol, DC);
                if (node->next != NULL)
                    fprintf(file, "\n");
            }
        }

        node = node->next;
        DC++;
    }
	
	set_ending_to_file_name(fileName, "as");
    fclose(file);
}

void write_to_file_entry(symbol_table *symbol_table, char fileName[]){
	int i;
    symbol *symbol;
    FILE *file1 = NULL;

	set_ending_to_file_name(fileName, "ent");
	
	for (i = 0; i < symbol_table->size; i++) {
		if (symbol_table->symbols[i].type == ENTRY_) {
			symbol = get_symbol_address_from_symbol_name(symbol_table, symbol_table->symbols[i].label);
			if (symbol != NULL) {
				if (file1 == NULL){
					file1 = fopen(fileName, "w");
				}

				fprintf(file1, "%s  %d\n", symbol->label, symbol->address);
			}
		}
	}
	
	set_ending_to_file_name(fileName, "as");
    fclose(file1);
}*/


void write_to_file_external(word_data *list1, word_data *list2, char fileName[], int IC, int DC){
	int current_word_line = 0;
	FILE *file;

	set_ending_to_file_name(fileName, "ob");
	if (IC == 0)
		return;
	file = fopen(fileName, "w");
	if (file == NULL)
		return;
	fprintf(file, "%5d %d\n", IC, DC);
	for (; current_word_line < IC; current_word_line++)
		fprintf(file, "%04d %05d\n", current_word_line + 100, int_to_octal(list1[current_word_line].word));

	for (current_word_line = 0; current_word_line < DC; current_word_line++) {
		fprintf(file, "%04d %05d", current_word_line + 100 + IC, int_to_octal(list2[current_word_line].word));
		if (current_word_line + 1 < DC)
			fprintf(file, "\n");
	}
/*
	while (node!=NULL){
		fprintf(file, "%04d %05d", DC, int_to_octal(node->word));
		if(node->next!=NULL)
			fprintf(file, "\n");

		node = node->next;
		current_word_line++;
	}
*/

	set_ending_to_file_name(fileName, "as");
	fclose(file);
}

void initialize_new_file_name(file *file_, error_array *error, char name[]) {
	file_->filename = (char*)use_malloc(sizeof(char) * (strlen(name) + 4), error);
    if (error->importance != NO_ERROR)
        return;
	
    strcpy(file_->filename, name);
	file_->filename[strlen(name)] = '.';
}

void set_ending_to_file_name(char *fileName, char ending[]){
	strcpy(strchr(fileName, '.') + 1, ending);
}
