#include "../header/first_pass.h"
#include "../header/consts.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void first_pass(char *file_name, error_array *error) {
	int IC = 0, DC = 0, label_amount = 0, extern_amount = 0, entry_amount = 0;

	/* Declare pointers to different data types */
	word_data *commands = NULL, *instructions = NULL;

	symbol_address *entries = NULL, *symbol_table = NULL, **externals = NULL;

	command_data command;
	instruction_data instruction;

	char line[MAX_LINE_LENGTH];
	char *workable_line, *start_workable_line;
	char symbol[MAX_SYMBOL_SIZE];
	start_workable_line = workable_line = (char*)use_malloc(sizeof(char) * MAX_LINE_LENGTH, error);
	
	FILE *file;
	int line_number = 0;
	int offset;

	int symbol_defined = 0;

	set_ending_to_file_name(file_name, "am");
	file = fopen(file_name, "r");
	if (file == NULL) {
		add_error(error, FILE_NOT_FOUND, 0, 0, 0, CRITICAL, file_name, 0);
		return;
	}
	
	
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL && error->importance != CRITICAL){
		workable_line = start_workable_line;
		error->importance = NO_ERROR;
		line_number++;
		offset = 0;
		
		if (is_empty_line(line) || is_comment_line(line))
			continue;
		line[strlen(line) - 1] = '\0';
		strcpy(workable_line, line);

		workable_line = strtok(workable_line, " \t");
		symbol_defined = is_valid_symbol(workable_line);
		if (symbol_defined == 2) {
			add_error(error, INVALID_SYMBOL_NAME, line_number, 0,
					  0 + strlen(workable_line), WARNING, workable_line, 0);
			continue;
		}
		if (symbol_defined == 1) {
			workable_line[strlen(workable_line) - 1] = '\0';
			strcpy(symbol, workable_line);
			workable_line = strtok(NULL, " \t");
			label_amount++;
		}
		
		if (is_directive(workable_line)) {

			if (strcmp(workable_line, ".string")==0 || strcmp(workable_line, ".data")==0) {
				if (workable_line[1] == 's') {
					workable_line = strtok(NULL, " \t\n");
					read_string(&workable_line, line, &instruction, error, line_number);
				} else {
					workable_line += strlen(workable_line) + 1;
					read_data(&workable_line, line, &instruction, error, line_number);
				}
				if (symbol_defined == 1) {
					instruction.label = (char *) use_malloc(sizeof(char) * strlen(symbol), error);
					if (add_symbol(&symbol_table, label_amount, DC, line_number, instruction.args, 1, error) == 0)
						continue;
				}

				if (error->importance != NO_ERROR)
					continue;

				instructions = (word_data *)use_realloc(instructions, sizeof(word_data) * (instruction.size + (DC)), error);
				add_instruction_to_words(instructions, instruction, line_number, error, &DC);
			} else if (strcmp(workable_line, ".extern")==0 || strcmp(workable_line, ".entry")==0) {
				if (symbol_defined == 1) {
					add_error(error, SYMBOL_IN_EXTERNAL_OR_ENTRY, line_number, 0,
							  strlen(workable_line), CRITICAL, line, 0);
					continue;
				}

				instruction.is_extern = workable_line[2] == 'x';

				if (read_extern_or_entry_symbol(&workable_line, line, &instruction, error, line_number) == 0)
					continue;

				if (instruction.is_extern) {
					extern_amount++;
					label_amount++;

					if (add_symbol(&symbol_table, label_amount, 1, line_number, instruction.args, 0, error) == 0)
						continue;

					externals = realloc(externals, sizeof(symbol_address *) * extern_amount);
					if (externals == NULL) {
						add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, 0, 0);
						return;
					}

					externals[extern_amount - 1] = (symbol_table + label_amount - 1);
				} else {
					entry_amount++;
					if (add_symbol(&entries, entry_amount, 0, line_number, instruction.args, 0, error) == 0) {
						continue;
						printf("extern: \n");
					}
				}
				
			} else {
				add_error(error, INVALID_DIRECTIVE_TYPE, line_number, 0, 0 + strlen(workable_line), CRITICAL, line, 0);
				continue;
			}
		} else {
			if (symbol_defined == 1) {
				if (add_symbol(&symbol_table, label_amount, IC, line_number, instruction.args, 0, error) == 0)
					continue;
/*
				workable_line = strtok(NULL, " \t\n");
*/
			}
			command.opcode = get_opcode_from_string(workable_line);

			
			if (command.opcode < 0) {
				add_error(error, INVALID_OPCODE, line_number, 0, 0 + strlen(workable_line), CRITICAL, line, 0);
				continue;
			}
			if (read_command_variables(&workable_line, line, &command, error, line_number) == 0)
				continue;
			
			int k = get_amount_of_words_from_command(command);
			if (k == 3 && (command.source.type == REGISTER_INDIRECT || command.source.type == REGISTER_DIRECT) && (command.destination.type == REGISTER_INDIRECT || command.destination.type == REGISTER_DIRECT))
				k = 2;
			commands = (word_data *)use_realloc(commands, sizeof(word_data) * (k + IC), error);


			add_command_to_words(commands, command, line_number, error, &IC);
			IC += get_amount_of_words_from_command(command);
		}
	}
	write_to_file_external(commands, instructions, file_name, IC, DC);
	
	
	free(externals);
	free(commands);
	free(instructions);
	fclose(file);
}

/*
void format_line(char line[LINE_SIZE], word_list_block *current_line_word_block, symbol_table *table, int *IC, int *DC, error_array *error, int line_number) {
	int offset = 0;
	symbol *symbol;
	line_data data;
	data.directive = NULL;
	data.command = NULL;
	symbol = get_symbol(line, &offset, error);
	
	if (symbol == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
		return;
	}

	line_data_set(&data, offset, line, symbol, error, line_number);
	if (error->importance != NO_ERROR) {
		free(symbol);
		return;
	}
	line_data_to_word_list_block(current_line_word_block, &data, error, line_number, line, DC, IC);


	if (symbol->label[0] != '\0') {
		if (data.directive != NULL && data.directive->type == EXTERN)
			add_symbol(table, symbol->label, error, 0, EXTERNAL);
		else
			add_symbol(table, symbol->label, error, (*IC) + (*DC), symbol->type);
	}
	free(symbol);
}*/
