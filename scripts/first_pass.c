#include "../header/first_pass.h"
#include "../header/second_pass.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void first_pass(char *file_name, error *error, macro *macros, int number_of_macros) {
	int IC = 0, DC = 0, label_amount = 0, extern_amount = 0, entry_amount = 0, found_macro;
	/* Declare pointers to different data types */
	word_data *commands = NULL, *instructions = NULL;

	symbol_address *entries = NULL, *symbol_table = NULL;

	command_data command;
	instruction_data instruction;

	char line[MAX_LINE_LENGTH];
	char symbol[MAX_SYMBOL_SIZE];
	FILE *file;
	int line_number = 0;

	int symbol_defined;

	char start_workable_line[MAX_LINE_LENGTH];
	char *workable_line = start_workable_line;
	if (error->importance != NO_ERROR)
		return;

	set_ending_to_file_name(file_name, "am");
	file = fopen(file_name, "r");
	if (file == NULL) {
		print_system_error(FILE_NOT_FOUND_MESSAGE);
		error->importance = CRITICAL;
		return;
	}
	
	
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL && error->importance != CRITICAL) {
		workable_line = start_workable_line;
		symbol[0] = '\0';
		error->importance = NO_ERROR;
		line_number++;

		if (is_empty_line(line) || is_comment_line(line))
			continue;

		workable_line = strchr(line, '\r');
		if (workable_line != NULL)
			*workable_line = '\0';

		workable_line = strchr(line, '\n');
		if (workable_line != NULL)
			*workable_line = '\0';
		
		workable_line = start_workable_line;
		strcpy(workable_line, line);

		workable_line = strtok(workable_line, " \t");
		symbol_defined = is_valid_symbol(workable_line);
		if (symbol_defined == 0) {
			if (strlen(workable_line) > MAX_SYMBOL_SIZE)
				print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE, SYMBOL_IS_TOO_LONG_DESCRIPTION, line, line_number, workable_line - start_workable_line, workable_line - start_workable_line + strlen(workable_line), workable_line - start_workable_line + MAX_SYMBOL_SIZE, 0, error);
			else
				print_general_error(INVALID_SYMBOL_NAME_MESSAGE, INVALID_SYMBOL_NAME_DESCRIPTION, line, line_number, workable_line - start_workable_line, workable_line - start_workable_line + strlen(workable_line), 0, 0, error);
			error->importance = WARNING;
			continue;
		}
		if (symbol_defined == 1) {
			found_macro = search_macro_by_name(workable_line, macros, number_of_macros);
			if (found_macro != -1) {
				print_macro_b_label_same(line, line_number, macros[found_macro].line_number, error, workable_line);
				error->importance = WARNING;
				continue;
			}
			strcpy(symbol, workable_line);
			workable_line = strtok(NULL, " \t");
		}

		if (workable_line == NULL) {
			print_general_error_no_quoting(UNDEFINED_OPCODE_MESSAGE, UNDEFINED_OPCODE_DESCRIPTION, line, line_number, strlen(symbol)+2, strlen(symbol)+2, strlen(symbol)+2, 0, error);
			error->importance = WARNING;
			continue;
		}


		if (is_directive(workable_line)) {
			if (strcmp(workable_line, ".string") == 0 || strcmp(workable_line, ".data") == 0) {
				if (workable_line[1] == 's') {
					workable_line += strlen(workable_line) + 1;
					skip_spaces_and_tabs(&workable_line);

					read_string(&workable_line, line, &instruction, error, line_number);
				} else {
					workable_line += strlen(workable_line) + 1;
					read_data(&workable_line, line, &instruction, error, line_number, start_workable_line);
				}
				if (symbol_defined == 1 && add_symbol(&symbol_table, &label_amount, DC + 100, line_number, symbol, 1, error, 0) == 0)
					continue;

				if (error->importance != NO_ERROR)
					continue;

				instructions = (word_data *) use_realloc(instructions, sizeof(word_data) * (instruction.size + (DC)), error);
				if (instructions == NULL)
					continue;

				add_instruction_to_words(instructions, instruction, line_number, &DC);
				if (instruction.numbers != NULL)
					free(instruction.numbers);
				
				instruction.numbers = NULL;
			} else if (strcmp(workable_line, ".extern") == 0 || strcmp(workable_line, ".entry") == 0) {
				
				if (symbol_defined == 1)
					print_general_error(SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE, SYMBOL_IN_EXTERNAL_OR_ENTRY_DESCRIPTION, line, line_number, strstr(line, symbol) - line, strstr(line, symbol) - line + strlen(symbol), 0, 0, error);

				instruction.is_extern = workable_line[2] == 'x';

				if (read_extern_or_entry_symbol(&workable_line, line, &instruction, error, line_number) == 0)
					continue;

				if (instruction.is_extern) {
					extern_amount++;
					if (add_symbol(&symbol_table, &label_amount, instruction.is_extern, line_number, instruction.args, 0, error, 1) == 0)
						continue;
				}
				else if (add_symbol(&entries, &entry_amount, instruction.is_extern, line_number, instruction.args, 0, error, 0) == 0)
					continue;

			} else {
				if (strlen(workable_line) == 1)
					print_general_error_no_quoting(INSTRUCTION_TYPE_MISSING_MESSAGE, INSTRUCTION_TYPE_MISSING_DESCRIPTION, line, line_number, workable_line - start_workable_line + 1, workable_line - start_workable_line + strlen(workable_line) + 1, workable_line - start_workable_line + 2, 0, error);
				else
					print_general_error(INVALID_DIRECTIVE_TYPE_MESSAGE, INVALID_DIRECTIVE_TYPE_DESCRIPTION, line, line_number, workable_line - start_workable_line + 1, workable_line - start_workable_line + strlen(workable_line) + 1, 0, 0, error);
				error->importance = WARNING;
				continue;
			}
			workable_line += strlen(workable_line);
			
		} else {
			if (symbol_defined == 1 && add_symbol(&symbol_table, &label_amount, IC + 100, line_number, symbol, 0, error, 0) == 0)
				continue;
			command.opcode = get_opcode_from_string(workable_line);

			if (command.opcode < 0) {
				print_general_error(INVALID_OPCODE_MESSAGE, INVALID_OPCODE_DESCRIPTION, line, line_number, workable_line - start_workable_line + 1, workable_line - start_workable_line + strlen(workable_line) + 1, 0, 0, error);
				error->importance = WARNING;
				continue;
			}
			
			if ((command.opcode != STOP && command.opcode != RTS) && read_command_variables(&workable_line, line, &command, error, line_number, start_workable_line) == 0)
				continue;
			
			commands = (word_data *)use_realloc(commands, sizeof(word_data) * (get_amount_of_words_from_command(command) + IC), error);
			if (error->importance == CRITICAL)
				continue;

			add_command_to_words(commands, command, error, &IC, line_number);
			
			IC += get_amount_of_words_from_command(command);
			if (command.opcode == STOP || command.opcode == RTS){
				workable_line = strtok(NULL, "\r\n");
				if (workable_line == NULL)
					continue;
			}
		}
		skip_spaces_and_tabs(&workable_line);
		
		if (*workable_line != '\0')
			print_general_error_no_quoting(EXTRA_TEXT_MESSAGE, EXTRA_TEXT_DESCRIPTION, line, line_number, workable_line - start_workable_line + 1, workable_line - start_workable_line + strlen(workable_line) + 1, 0, 0, error);
	}
	fclose(file);
	if (error->importance != CRITICAL){
		update_table_by(symbol_table, IC, label_amount, 1, error);
		second_pass(commands, instructions, entries, symbol_table, error, IC, DC, label_amount, entry_amount, extern_amount);
	}
	
	if (error->error_single_file_count == 0) {
		write_to_file_object(commands, instructions, file_name, IC, DC);
		if (entry_amount > 0)
			write_to_file_entry(entries, entry_amount, symbol_table, label_amount, file_name);
		if (extern_amount > 0)
			write_to_file_external(commands, IC, file_name);
	}
	
	if (symbol_table != NULL)
		free(symbol_table);
	if (entries != NULL)
		free(entries);

	free_word_data_list(commands, IC);
	free_word_data_list(instructions, DC);
}
