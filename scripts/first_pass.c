#include "../header/first_pass.h"
#include "../header/second_pass.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/**
 * @brief Handles a directive type line.
 *
 * This function processes a line of code that represents a directive.
 * 
 * at first, its extracts the directive type from the line and validates it.
 * If the directive type is invalid, an error is printed and the function returns 0.
 * 
 * else if the directive type is external or entry:
 * - if a symbol is defined on the start of the line, an error is printed and the function returns 0.
 * - it reads the external or entry symbol from the line.
 * - it validates the symbol, if the symbol is invalid, an error is printed and the function returns 0.
 * - if the directive type is .extern, it adds the symbol to the symbol table with the 'external' and address set to 1, and increments the count of external symbols.
 * - else the directive type is .entry, it adds the symbol to the entries table with the address set to 0, and increments the count of entry symbols.
 * 
 * else the directive type is .data or .string
 * - if the directive type is .string, it will read the string from the line, if there is an error, the function will return 0.
 * - else the directive type is .data, it will read the data from the line, if there is an error, the function will return 0.
 * - if a symbol is defined on the line, it will add the symbol to the symbol table with the DC(data counter) value, and increments the DC, if there is an error, the function will return 0.
 * - it will add the data to the data words list, if there is an error, the function will return 0.
 *
 * @param line A pointer to the original line of assembly code.
 * @param workable_line A pointer to a pointer to the current working position within the line.
 * @param start_workable_line A pointer to the beginning of the workable portion of the line.
 * @param symbol A pointer to a buffer containing the symbol, if defined on the line.
 * @param symbol_defined A flag indicating whether a symbol is defined on the line (1) or not (0).
 * @param DC A pointer to the data counter.
 * @param label_amount A pointer to the count of labels in the symbol table.
 * @param line_number The line number of the current line being processed.
 * @param data_words A pointer to the list of data words.
 * @param symbol_table A pointer to the symbol table.
 * @param entries A pointer to the entries table.
 * @param entry_amount A pointer to the count of entry symbols.
 * @param extern_amount A pointer to the count of external symbols.
 * @param error A pointer to an error structure to report any errors encountered.
 * @param directive A pointer to a `directive_data` structure to store the parsed directive information.
 * @return 1 if the directive line was processed successfully, 0 otherwise.
 */
int handle_directive_type_line(char *line, char **workable_line, char *start_workable_line, char *symbol,
							   int symbol_defined, int *DC, int *label_amount, int line_number, word_data **data_words,
							   symbol_address **symbol_table, symbol_address **entries, int *entry_amount,
							   int *extern_amount, error *error, directive_data *directive, macro *macros,
							   int number_of_macros);


/**
 * @brief Handle a command type line, process the command and add it to the command words list.
 *
 * This function processes a line of code that represents a command. It performs the following tasks:
 * 1. Adds a symbol to the symbol table if defined on the beginning of the line.
 * 2. Extracts the opcode from the line and validates it, if the opcode is invalid, an error is printed and the function returns 0.
 * 3. Reads and validates command variables based on the opcode.
 * 4. Translates the command data into machine code words and adds them to the command words list.
 * 5. Updates the instruction counter(IC) based on the amount of words needed for the command.
 *
 * @param line A pointer to the original line of assembly code.
 * @param workable_line A pointer to a pointer to the current working position within the line.
 * @param start_workable_line A pointer to the beginning of the workable portion of the line.
 * @param symbol A pointer to a buffer containing the symbol, if defined on the line.
 * @param symbol_defined A flag indicating whether a symbol is defined on the line (1) or not (0).
 * @param IC A pointer to the instruction counter.
 * @param label_amount A pointer to the count of labels in the symbol table.
 * @param line_number The line number of the current line being processed.
 * @param command_words A pointer to the list of machine code words.
 * @param error A pointer to an error structure to report any errors encountered.
 * @param symbol_table A pointer to the symbol table.
 * @param command A pointer to a command_data structure to store the parsed command information.
 * @return 1 if the line was processed successfully, 0 otherwise.
 */
int handle_commend_type_line(char *line, char **workable_line, char *start_workable_line, char *symbol,
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
	directive.numbers = NULL;
	/*set file name to be the source file name
	 * open file, if file not found, print error and return*/
	set_ending_to_file_name(file_name, SOURCE_FILE_AFTER_POST_ASSEMBLER_ENDING);
	file = fopen(file_name, "r");
	if (file == NULL) {
		/*if file not found, print error and return*/
		print_system_error(FILE_NOT_FOUND_MESSAGE, error, CANCELLATION);
		return;
	}

	/*while there are lines in the file and no critical error, continue reading lines*/
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL && error->importance != CRITICAL &&
		   error->importance != CANCELLATION) {
		/*incrementing the line number*/
		line_number++;

		/*if the line is empty or a comment, continue to the next line*/
		if (is_empty_line(line) || is_comment_line(line)) {
			continue;
		}

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
				print_general_error_no_quoting(MISSING_SYMBOL_MESSAGE, MISSING_SYMBOL_DESCRIPTION, line, line_number,
											   workable_line - start_workable_line,
											   workable_line - start_workable_line + 1,
											   workable_line - start_workable_line, -1, error);
			}
			/*if the symbol is not valid, print error*/
			else {
				print_general_error(INVALID_SYMBOL_NAME_MESSAGE, INVALID_SYMBOL_NAME_DESCRIPTION, line, line_number,
									workable_line - start_workable_line,
									workable_line - start_workable_line + strlen(workable_line), -1, -1, error);
			}

			/*continue to the next line*/
			continue;
		}

		/*if symbol_defined is 1(symbol valid)*/
		if (symbol_defined == 1) {
			/*check if the symbol is a macro by searching the symbol in the macros array*/
			found_macro = search_macro_by_name(workable_line, macros, number_of_macros);
			if (found_macro != -1) {
				/*if the symbol is a macro, print error and continue to the next line*/
				print_macro_b_label_same(line, line_number, macros[found_macro].line_number, error, workable_line);
				continue;
			}
			/*check if the symbol is a saved word*/
			switch (is_a_saved_word(workable_line)) {
				case SAVED_WORD_OPCODE:
					print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE, OPCODE_NAME_AS_SYMBOL_NAME_DESCRIPTION,
												   line, line_number, workable_line - start_workable_line,
												   workable_line - start_workable_line + strlen(workable_line), -1, -1,
												   error);
					continue;
				case SAVED_WORD_DIRECTIVE:
					print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE,
												   DIRECTIVE_NAME_AS_SYMBOL_NAME_DESCRIPTION, line, line_number,
												   workable_line - start_workable_line,
												   workable_line - start_workable_line + strlen(workable_line), -1, -1,
												   error);
					continue;
				case SAVED_WORD_REGISTER:
					print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE,
												   REGISTER_NAME_AS_SYMBOL_NAME_DESCRIPTION, line, line_number,
												   workable_line - start_workable_line,
												   workable_line - start_workable_line + strlen(workable_line), -1, -1,
												   error);
					continue;
				case SAVED_WORD_MACRO: {
					print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE,
												   SYMBOL_NAME_CANNOT_BE_SAVED_WORD_DESCRIPTION, line, line_number,
												   workable_line - start_workable_line,
												   workable_line - start_workable_line + strlen(workable_line), -1, -1,
												   error);
					continue;
				}
				case NON_SAVED_WORD:
					break;
			}

			/*copy the symbol to the symbol string*/
			strcpy(symbol, workable_line);
			workable_line = strtok(NULL, " \t");
		}

		/*if first non-symbol word is non exist, print error and continue to the next line*/
		if (workable_line == NULL) {
			print_general_error_no_quoting(UNDEFINED_OPCODE_MESSAGE, UNDEFINED_OPCODE_DESCRIPTION, line, line_number,
										   strlen(symbol) + 2, strlen(symbol) + 2, strlen(symbol) + 2, -1, error);
			continue;
		}


		/*if the first non-symbol word in the line is a directive, handle the directive, if there is an error, continue to the next line*/
		if (is_directive(workable_line)) {
			if (handle_directive_type_line(line, &workable_line, start_workable_line, symbol, symbol_defined, &DC,
										   &label_amount, line_number, &data_words, &symbol_table, &entries,
										   &entry_amount, &extern_amount, error, &directive, macros, number_of_macros) == 0) {
				continue;
			}
		}
		/*if the first non-symbol word in the line is not a directive, handle the command, if there is an error, continue to the next line*/
		else if (handle_commend_type_line(line, &workable_line, start_workable_line, symbol, symbol_defined, &IC,
										  &label_amount, line_number, &command_words, error, &symbol_table, &command) == 0) {
			continue;
		}

		/*if there is any character left in the line, print an extra text error, and continue to the next line*/
		if (is_empty_line(workable_line) == 0) {
			print_general_error_no_quoting(EXTRA_TEXT_MESSAGE, EXTRA_TEXT_DESCRIPTION, line, line_number,
										   workable_line - start_workable_line,
										   workable_line - start_workable_line + strlen(workable_line), -1, -1, error);
		}

		/*if there is no critical or cancellation error, check if the memory is overflowed,
		 * overflowed memory is when the IC + DC is bigger than the maximum number of total words - OS_SAVED_MEMORY_CELl.
		 * if the memory is overflowed, print an error message. */
		if (error->importance != CRITICAL && error->importance != CANCELLATION && IC + DC > MAXIMUM_NUMBER_OF_TOTAL_WORDS - OS_SAVED_MEMORY_CELLS) {
			print_system_error(MEMORY_OVERFLOW_MESSAGE, error, WARNING);
		}
	}

	/*close the file*/
	fclose(file);
	
	/*if there is no critical error, add to every symbol address with the data flag on, IC + OS_SAVED_MEMORY_CELLS, and continue to the second pass*/
	if (error->importance != CRITICAL && error->importance != CANCELLATION) {
		update_table_by(symbol_table, IC + OS_SAVED_MEMORY_CELLS, label_amount, 1);
		second_pass(command_words, entries, symbol_table, error, IC, label_amount, entry_amount, extern_amount);
	}


	/*if there isn't any error in the file(the error_single_file_count is 0), write the data to the files:
	 * -write the command words and data words to the object file (.ob).
	 * -write the entries to the entries file (.ent), if there are any.
	 * -write the labels who marked as external to the externals file (.ext), if there are any.
	 */
	if (error->error_single_file_count == 0) {
		write_to_file_object(command_words, data_words, file_name, IC, DC, error);
		write_to_file_entry(entries, entry_amount, symbol_table, label_amount, file_name, error);
		write_to_file_external(command_words, IC, file_name, extern_amount, error);
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
							   int *extern_amount, error *error, directive_data *directive, macro *macros,
							   int number_of_macros) {
	int found_macro;
	/*get the directive type from the string*/
	directive->type = get_directive_type_from_string(*workable_line);

	/*if the directive type is -1, print an error and return 0*/
	if (directive->type == -1) {
		/*if the word is empty, print an error that the directive type is missing*/
		if (strlen(*workable_line) == 1) {
			print_general_error_no_quoting(DIRECTIVE_TYPE_MISSING_MESSAGE, DIRECTIVE_TYPE_MISSING_DESCRIPTION, line,
										   line_number, *workable_line - start_workable_line + 1,
										   *workable_line - start_workable_line + strlen(*workable_line) + 1,
										   *workable_line - start_workable_line + 2, -1, error);
		}
			/*else print an error that the directive type is invalid*/
		else {
			print_general_error(INVALID_DIRECTIVE_TYPE_MESSAGE, INVALID_DIRECTIVE_TYPE_DESCRIPTION, line, line_number,
								*workable_line - start_workable_line,
								*workable_line - start_workable_line + strlen(*workable_line), -1, -1, error);
		}
		return 0;
	}

	/*if the directive type is extern or entry, handle the extern or entry symbol*/
	if (directive->type == EXTERN || directive->type == ENTRY) {
		/*if the symbol is already defined, print an error and return 0*/
		if (symbol_defined == 1) {
			print_general_error(SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE, SYMBOL_IN_EXTERNAL_OR_ENTRY_DESCRIPTION, line,
								line_number, strstr(line, symbol) - line, strstr(line, symbol) - line + strlen(symbol),
								-1, -1, error);
			return 0;
		}

		/*if the symbol is not defined, read the extern or entry symbol*/
		if (read_extern_or_entry_symbol(workable_line, line, directive, error, line_number, start_workable_line) == 0) {
			return 0;
		}


		/*check if the symbol is a macro by searching the symbol in the macros array*/
		found_macro = search_macro_by_name(directive->args, macros, number_of_macros);
		if (found_macro != -1) {
			/*if the symbol is a macro, print error and continue to the next line*/
			print_macro_b_label_same(line, line_number, macros[found_macro].line_number, error, directive->args);
			return 0;
		}
		switch (is_a_saved_word(directive->args)) {
			case SAVED_WORD_OPCODE:
				print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE, OPCODE_NAME_AS_SYMBOL_NAME_DESCRIPTION,
											   line, line_number, strstr(line, directive->args) - line,
											   strstr(line, directive->args) - line + strlen(directive->args), -1, -1,
											   error);
				return 0;
			case SAVED_WORD_DIRECTIVE:
				print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE,
											   DIRECTIVE_NAME_AS_SYMBOL_NAME_DESCRIPTION, line, line_number,
											   strstr(line, directive->args) - line,
											   strstr(line, directive->args) - line + strlen(directive->args), -1, -1,
											   error);
				return 0;
			case SAVED_WORD_REGISTER:
				print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE, REGISTER_NAME_AS_SYMBOL_NAME_DESCRIPTION, line, line_number,
											   strstr(line, directive->args) - line,
											   strstr(line, directive->args) - line + strlen(directive->args), -1, -1,
											   error);
				return 0;
			case SAVED_WORD_MACRO: {
				print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE,
											   SYMBOL_NAME_CANNOT_BE_SAVED_WORD_DESCRIPTION, line, line_number,
											   strstr(line, directive->args) - line,
											   strstr(line, directive->args) - line + strlen(directive->args), -1, -1,
											   error);
				return 0;
			}
			case NON_SAVED_WORD:
				break;
		}
		

		/*if the directive type is extern, add the symbol to the symbol table with the extern flag on, and increment the extern amount*/
		if (directive->type == EXTERN) {
			(*extern_amount)++;

			/*add the symbol to the symbol table, if there is an error, return 0*/
			if (add_symbol(symbol_table, label_amount, 1, line_number, directive->args, 0, error, 1) == 0) {
				return 0;
			}
		}
			/*if the directive type is entry, add the symbol to the entries table, and increment the entry amount, if there is an error, return 0*/
		else if (add_symbol(entries, entry_amount, 0, line_number, directive->args, 0, error, 0) == 0) {
			return 0;
		}
		/*else the directive type is string or data type*/
	} else {
		/*move the workable line pointer to the next word and skip spaces and tabs*/
		*workable_line += strlen(*workable_line) + 1;
		skip_spaces_and_tabs(workable_line);

		/*if the directive type is string, read the string, if there is an error, return 0*/
		if (directive->type == STRING) {
			if (read_string(workable_line, line, directive, error, line_number, start_workable_line) == 0) {
				return 0;
			}
		}
			/*else the directive type is data, read the data, if there is an error, return 0*/
		else if (read_data(workable_line, line, directive, error, line_number, start_workable_line) == 0) {
			handle_free(directive->numbers);
			return 0;
		}

		/*if there is a symbol defined, add the symbol to the symbol table with the DC value, and increment the DC, if there is an error,
		 * try to free the directive->numbers list(if necessary) and return 0.
		 */
		if (symbol_defined == 1 &&
			add_symbol(symbol_table, label_amount, *DC, line_number, symbol, 1, error, 0) == 0) {
			handle_free(directive->numbers);
			return 0;
		}

		/*add the data to the data words list, if there is an error, return 0
		 * try to free the directive->numbers list(if necessary) and return 0.
		 */
		if (add_data_to_words(data_words, *directive, line_number, DC, error) == 0) {
			handle_free(directive->numbers);
			return 0;
		}

		/*try to free the directive->numbers list(if necessary)*/
		handle_free(directive->numbers);
	}

	/*return 1*/
	return 1;
}

int handle_commend_type_line(char *line, char **workable_line, char *start_workable_line, char *symbol,
							 int symbol_defined, int *IC, int *label_amount, int line_number, word_data **command_words,
							 error *error, symbol_address **symbol_table, command_data *command) {
	/*if the symbol is defined, add the symbol to the symbol table with the IC + OS_SAVED_MEMORY_CELLS value, and increment the IC, if there is an error, return 0*/
	if (symbol_defined == 1 &&
		add_symbol(symbol_table, label_amount, *IC + OS_SAVED_MEMORY_CELLS, line_number, symbol, 0, error, 0) == 0) {
		return 0;
	}

	/*get the opcode from the string and save it in the command->opcode, if the opcode is -1, print an error and return 0*/
	command->opcode = get_opcode_from_string(*workable_line);
	if (command->opcode == -1) {
		print_general_error(INVALID_OPCODE_MESSAGE, INVALID_OPCODE_DESCRIPTION, line, line_number,
							*workable_line - start_workable_line,
							*workable_line - start_workable_line + strlen(*workable_line), -1, -1, error);
		return 0;
	}

	/*if the opcode is not stop or rts(no variables), read the command variables, if there is an error, return 0*/
	if (command->opcode != STOP && command->opcode != RTS &&
		read_command_variables(workable_line, line, command, error, line_number, start_workable_line) == 0) {
		return 0;
	}

	/*translate the command data into words and add them to the command words list, if there is an error, return 0*/
	if (add_command_to_words(command_words, *command, error, IC, line_number) == 0) {
		return 0;
	}

	/*increment the IC by the amount of words needed for the command*/
	*IC += get_amount_of_words_from_command(*command);

	/*if the command is stop or rts, move the workable line pointer to the next word(from the end of the current word to the end of the line)*/
	if (command->opcode == STOP || command->opcode == RTS) {
		*workable_line = strtok(NULL, "\r\n");

		if (is_empty_line(*workable_line) == 0) {
			print_general_error_no_quoting(EXTRA_PARAMETER_MESSAGE, TOO_MANY_PARAMETERS_DESCRIPTION, line, line_number,
										   *workable_line - start_workable_line,
										   *workable_line - start_workable_line + strlen(*workable_line), -1, -1, error);
			return 0;
		}
			
	}

	/*return 1*/
	return 1;
}