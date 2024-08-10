#include "../header/first_pass.h"
#include "../header/second_pass.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int handle_directive_type_line(char *line, char **workable_line, char *start_workable_line, char *symbol,
							   int symbol_defined, int *DC, int *label_amount, int line_number, word_data **data_words,
							   symbol_address **symbol_table, symbol_address **entries, int *entry_amount,
							   int *extern_amount, error *error, directive_data *directive);

int handle_comment_type_line(char *line, char **workable_line, char *start_workable_line, char *symbol,
							 int symbol_defined, int *IC, int *label_amount, int line_number, word_data **command_words,
							 error *error, symbol_address **symbol_table, command_data *command);

void first_pass(char *file_name, error *error, macro *macros, int number_of_macros) {
	int IC = 0, DC = 0, label_amount = 0, extern_amount = 0, entry_amount = 0, found_macro, symbol_defined, line_number = 0;

	/*declaration of machine code image*/
	word_data *command_words = NULL, *data_words = NULL;

	/*declaration of symbol table*/
	symbol_address *entries = NULL, *symbol_table = NULL;

	/*declaration of directive and command data*/
	directive_data directive;
	command_data command;

	/*declaration of line and symbol and temp line, and pointer*/
	char line[MAX_LINE_LENGTH], symbol[MAX_SYMBOL_SIZE], start_workable_line[MAX_LINE_LENGTH], *workable_line = start_workable_line;

	/*declaration of file*/
	FILE *file;

	/*set file name to be the source file name
	 * open file, if file not found, print error and return*/
	set_ending_to_file_name(file_name, SOURCE_FILE_AFTER_POST_ASSEMBLER_ENDING);
	file = fopen(file_name, "r");
	if (file == NULL) {
		/*if file not found, print error and return*/
		print_system_error(FILE_NOT_FOUND_MESSAGE);
		error->importance = CRITICAL;
		return;
	}

	/*while there are lines in the file and no critical error, continue reading lines*/
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL && error->importance != CRITICAL) {
		/*incrementing the line number*/
		line_number++;

		/*if the line is empty or a comment, continue to the next line*/
		if (is_empty_line(line) || is_comment_line(line)) {
			continue;
		}

		/*returning tje workable_line pointer to the start_of_the_line*/
		workable_line = start_workable_line;

		/*clearing the symbol string*/
		symbol[0] = '\0';

		/*setting the error importance to no error*/
		error->importance = NO_ERROR;

		/*removing the \r and \n from the line*/
		workable_line = strchr(line, '\r');
		if (workable_line != NULL) {
			*workable_line = '\0';
		}

		workable_line = strchr(line, '\n');
		if (workable_line != NULL) {
			*workable_line = '\0';
		}

		/*setting the workable_line pointer to the start of the line,
		 *then copying the line to the workable_line*/
		workable_line = start_workable_line;
		strcpy(workable_line, line);

		/*getting the first word of the line*/
		workable_line = strtok(workable_line, " \t");

		/*checking if the first word is a symbol*/
		symbol_defined = is_valid_symbol(workable_line);

		/*if symbol_defined is 0(symbol not valid), print error and continue to the next line*/
		if (symbol_defined == 0) {
			/*if the symbol is too long, print error*/
			if (strlen(workable_line) > MAX_SYMBOL_SIZE) {
				print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE, SYMBOL_IS_TOO_LONG_DESCRIPTION, line,
											   line_number, workable_line - start_workable_line,
											   workable_line - start_workable_line + strlen(workable_line),
											   workable_line - start_workable_line + MAX_SYMBOL_SIZE, -1, error);
			}
			/*if the symbol is empty, print error*/
			else if (strlen(workable_line) == 0) {
				print_general_error_no_quoting(MISSING_SYMBOL, MISSING_SYMBOL_DESCRIPTION, line, line_number,
											   workable_line - start_workable_line, workable_line - start_workable_line + 1,
											   workable_line - start_workable_line, -1, error);
			}
			/*if the symbol is not valid, print error*/
			else {
				print_general_error(INVALID_SYMBOL_NAME_MESSAGE, INVALID_SYMBOL_NAME_DESCRIPTION, line, line_number,
									workable_line - start_workable_line,
									workable_line - start_workable_line + strlen(workable_line), -1, -1, error);
			}

			/*set the error importance to warning and continue to the next line*/
			error->importance = WARNING;
			continue;
		}

		/*if symbol_defined is 1(symbol valid)*/
		if (symbol_defined == 1) {
			/*check if the symbol is a macro by searching the symbol in the macros array*/
			found_macro = search_macro_by_name(workable_line, macros, number_of_macros);
			if (found_macro != -1) {
				/*if the symbol is a macro, print error and continue to the next line*/
				print_macro_b_label_same(line, line_number, macros[found_macro].line_number, error, workable_line);
				error->importance = WARNING;
				continue;
			}

			/*copy the symbol to the symbol string*/
			strcpy(symbol, workable_line);
			workable_line = strtok(NULL, " \t");
		}

		/*if first non-symbol word is non exist, print error and continue to the next line*/
		if (workable_line == NULL) {
			print_general_error_no_quoting(UNDEFINED_OPCODE_MESSAGE, UNDEFINED_OPCODE_DESCRIPTION, line, line_number,
										   strlen(symbol) + 2, strlen(symbol) + 2, strlen(symbol) + 2, -1, error);
			error->importance = WARNING;
			continue;
		}
		
		
		/*if the first non-symbol word in the line is a directive, handle the directive, if there is an error, continue to the next line*/
		if (is_directive(workable_line)) {
			if (handle_directive_type_line(line, &workable_line, start_workable_line, symbol, symbol_defined, &DC,
										   &label_amount, line_number, &data_words, &symbol_table, &entries,
										   &entry_amount, &extern_amount, error, &directive) == 0) {
				continue;
			}
		}
		/*if the first non-symbol word in the line is not a directive, handle the command, if there is an error, continue to the next line*/
		else if (handle_comment_type_line(line, &workable_line, start_workable_line, symbol, symbol_defined, &IC,
										 &label_amount, line_number, &command_words, error, &symbol_table, &command) == 0) {
				continue;
		}

		/*if there is any character left in the line, print an extra text error, and continue to the next line*/
		if (is_empty_line(workable_line) == 0) {
			print_general_error_no_quoting(EXTRA_TEXT_MESSAGE, EXTRA_TEXT_DESCRIPTION, line, line_number,
										   workable_line - start_workable_line,
										   workable_line - start_workable_line + strlen(workable_line), -1, -1, error);
		}
	}

	/*close the file*/
	fclose(file);

	/*if there is no critical error, add to every symbol address with the data flag on, IC, and continue to the second pass*/ 
	if (error->importance != CRITICAL) {
		update_table_by(symbol_table, IC, label_amount, 1, error);
		second_pass(command_words, data_words, entries, symbol_table, error, IC, DC, label_amount, entry_amount, extern_amount);
	}

	
	/*if there isn't any error in the file(the error_single_file_count is 0), write the data to the files:
	 * -write the command words and data words to the object file (.ob).
	 * -write the entries to the entries file (.ent), if there are any.
	 * -write the labels who marked as external to the externals file (.ext), if there are any.
	 */
	if (error->error_single_file_count == 0) {
		write_to_file_object(command_words, data_words, file_name, IC, DC);
		write_to_file_entry(entries, entry_amount, symbol_table, label_amount, file_name);
		write_to_file_external(command_words, IC, file_name, extern_amount);
	}

	
	/*free the memory of the command words, data words, symbol table, and entries, if needed*/
	handle_free(symbol_table);
	handle_free(entries);
	free_word_data_list(command_words, IC);
	free_word_data_list(data_words, DC);
}

int handle_directive_type_line(char *line, char **workable_line, char *start_workable_line, char *symbol,
							   int symbol_defined, int *DC, int *label_amount, int line_number, word_data **data_words,
							   symbol_address **symbol_table, symbol_address **entries, int *entry_amount,
							   int *extern_amount, error *error, directive_data *directive) {
	
	/*get the directive type from the string*/
	directive->type = get_directive_type_from_string(*workable_line);

	/*if the directive type is -1, print an error and return 0*/
	if (directive->type == -1) {
		/*if the word is empty, print an error that the directive type is missing, else print an error that the directive type is invalid, and return 0*/
		if (strlen(*workable_line) == 1) {
			print_general_error_no_quoting(DIRECTIVE_TYPE_MISSING_MESSAGE, DIRECTIVE_TYPE_MISSING_DESCRIPTION, line,
										   line_number, *workable_line - start_workable_line + 1,
										   *workable_line - start_workable_line + strlen(*workable_line) + 1,
										   *workable_line - start_workable_line + 2, -1, error);
		} else {
			print_general_error(INVALID_DIRECTIVE_TYPE_MESSAGE, INVALID_DIRECTIVE_TYPE_DESCRIPTION, line, line_number,
								*workable_line - start_workable_line,
								*workable_line - start_workable_line + strlen(*workable_line), -1, -1, error);
		}
		error->importance = WARNING;
		return 0;
	}

	if (directive->type == EXTERN || directive->type == ENTRY){
		if (symbol_defined == 1) {
			print_general_error(SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE, SYMBOL_IN_EXTERNAL_OR_ENTRY_DESCRIPTION, line,
								line_number, strstr(line, symbol) - line, strstr(line, symbol) - line + strlen(symbol),
								-1, -1, error);
		}

		if (read_extern_or_entry_symbol(workable_line, line, directive, error, line_number) == 0) {
			return 0;
		}

		if (directive->type == EXTERN) {
			(*extern_amount)++;

			if (add_symbol(symbol_table, label_amount, 1, line_number, directive->args, 0, error, 1) == 0) {
				return 0;
			}
		} else if (add_symbol(entries, entry_amount, 0, line_number, directive->args, 0, error, 0) == 0) {
			return 0;
		}
	}else{
		*workable_line += strlen(*workable_line) + 1;
		skip_spaces_and_tabs(workable_line);

		if (directive->type == STRING) {
			if (read_string(workable_line, line, directive, error, line_number, start_workable_line) == 0) {
				return 0;
			}
		} else if (read_data(workable_line, line, directive, error, line_number, start_workable_line) == 0) {
			return 0;
		}

		if (symbol_defined == 1 &&
			add_symbol(symbol_table, label_amount, *DC + IC_START_COUNT, line_number, symbol, 1, error, 0) == 0) {
			return 0;
		}

		if (add_data_to_words(data_words, *directive, line_number, DC, error) == 0) {
			return 0;
		}

		handle_free(directive->numbers);
	}
	
	return 1;
}

int handle_comment_type_line(char *line, char **workable_line, char *start_workable_line, char *symbol,
							 int symbol_defined, int *IC, int *label_amount, int line_number, word_data **command_words,
							 error *error, symbol_address **symbol_table, command_data *command) {
	if (symbol_defined == 1 &&
		add_symbol(symbol_table, label_amount, *IC + IC_START_COUNT, line_number, symbol, 0, error, 0) == 0) {
		return 0;
	}

	command->opcode = get_opcode_from_string(*workable_line);

	if (command->opcode == -1) {
		print_general_error(INVALID_OPCODE_MESSAGE, INVALID_OPCODE_DESCRIPTION, line, line_number,
							*workable_line - start_workable_line,
							*workable_line - start_workable_line + strlen(*workable_line), -1, -1, error);
		error->importance = WARNING;
		return 0;
	}

	if (command->opcode != STOP && command->opcode != RTS &&
		read_command_variables(workable_line, line, command, error, line_number, start_workable_line) == 0) {
		return 0;
	}

	if (add_command_to_words(command_words, *command, error, IC, line_number) == 0) {
		return 0;
	}

	*IC += get_amount_of_words_from_command(*command);
	if (command->opcode == STOP || command->opcode == RTS) {
		*workable_line = strtok(NULL, "\r\n");
		if (*workable_line == NULL) {
			return 0;
		}
	}
	return 1;
}