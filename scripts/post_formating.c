#include "../header/file.h"
#include "../header/post_formating.h"
#include <stdio.h>
#include <string.h>


/**
 * @brief Checks if a given line defines a macro.
 * 
 * This function compares the provided line with the predefined macro string.
 * 
 * @param line The string to check.
 * @return int 1 if the line is equal to MACRO, 0 otherwise.
 */
int is_line_macro(const char line[]);


/**
 * @brief Adds a new macro to the list of macros.
 * 
 * This function reallocates memory to accommodate a new macro, initializes its fields, and adds it to the list.
 * 
 * @param macro_name The name of the macro to add.
 * @param macros A pointer to the array of macros.
 * @param number_of_macros A pointer to the number of macros.
 * @param error A pointer to an error structure to handle memory allocation errors.
 * @param line_number The line number where the macro is defined.
 * @return int 1 if the macro is successfully added, 0 if memory allocation fails.
 */
int add_macro(char macro_name[], macro **macros, int *number_of_macros, error *error, int line_number);


/**
 * @brief Checks if a given line is the end of a macro.
 * 
 * This function compares the provided line with the predefined end macro string.
 * 
 * @param line The string to check.
 * @return int 1 if the line is equal to END_MACRO, 0 otherwise.
 */
int is_ending_macro(const char line[]);


/**
 * @brief Frees the memory allocated for the lines in each macro.
 * 
 * This function iterates through an array of macros and frees the memory allocated for the lines in each macro.
 * 
 * @param macros An array of macros.
 * @param number_of_macros The number of macros in the array.
 */
void free_macros_lines(macro *macros, int number_of_macros);


/**
 * @brief Validates if a given line is a valid macro name.
 * 
 * This function checks if the provided line is a valid macro name by:
 * - Checking if the line exceeds the maximum symbol size.
 * - Checking if the line is a saved word.
 * the function returns -1 if the line exceeds the maximum symbol size, -2 if the macro name is already defined,
 * otherwise, it returns the saved word type.
 * 
 * @param segment The string to check.
 * @return int - -1 if the line exceeds the maximum symbol size,
 * 			   - -2 if the macro name is already defined,
 * 			   - saved word type if the line is a saved word.
 */
int is_macro_valid(char segment[], macro *macros, int amount_of_macros, char line[], int line_number, error *error);


/**
 * @brief handles the macro definition line.
 * 
 * This function extracts the macro name from the line,
 * checks if the macro name is valid,
 * and adds the macro to the list of macros.
 * 
 * If the macro name is not valid, the function prints the appropriate error message.
 * 
 * @param line The line to process.
 * @param macro_exist A pointer to a flag indicating if a macro is being defined.
 * @param start_workable_line A string containing the original line of code.
 * @param workable_line A pointer to the current position within the line being processed.
 * @param macros A pointer to the array of macros.
 * @param number_of_macros A pointer to the number of macros.
 * @param number_of_rows The number of rows in the new file.
 * @param error A pointer to an error structure to report any error.
 * @param line_number The line number of the current line being processed.
 * @return int 1 if the macro is successfully added, 0 if memory allocation fails.
 */
int handle_macro_definition_line(char line[], int *macro_exist, char start_workable_line[], char **workable_line,
								 macro **macros, int *number_of_macros, int number_of_rows, error *error,
								 int line_number);


void post_formating(error *error, char file_name[], macro **macros, int *number_of_macros) {
	/*initialize the lines array to store the new file*/
	line_text *new_file = NULL;

	/*initialize the file pointer*/
	FILE *file;

	/*initialize the variables*/
	int macro_exist = 0, number_of_rows = 0, search_result, i, line_too_long = 0, line_number = 0, current_line_macro_flag = 0;

	/*initialize the line to read from the file and the workable line to manipulate*/
	char line[MAX_LINE_LENGTH], start_workable_line[MAX_LINE_LENGTH], *workable_line;

	/*set the file name to ".as" file*/
	set_ending_to_file_name(file_name, SOURCE_FILE_ENDING);
	/*open the file, if it doesn't exist, print an error and return*/
	file = fopen(file_name, "r");
	if (file == NULL) {
		print_system_error(FILE_NOT_FOUND_MESSAGE, error, CANCELLATION);
		return;
	}

	/*read the file line by line until the end of the file or a critical or cancellation error occurs*/
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL && error->importance != CRITICAL &&
		   error->importance != CANCELLATION) {
		/*check if the line is too long, if it is, print the start of the error message*/
		if (is_line_too_long(line) == 0) {
			/*print the start of the error message*/
			if (line_too_long == 0) {
				print_start_line_too_long_error(line_number + 1, error);
			}

			/*print the line to continue the error message*/
			printf("%s", line);
			/*set the line_too_long flag to 1*/
			line_too_long = 1;
			/*continue to the next line*/
			continue;
		}

		/*if the line is not too long, but the previous line was too long */
		if (line_too_long == 1) {
			/*print the last line to the error message*/
			printf("%s", line);
			/*add 1 to the line number, to count the too long line as one line*/
			line_number++;
			/*print the end of the error message*/
			print_end_line_too_long_error();
			/*set the line_too_long flag to 0*/
			line_too_long = 0;
			/*continue to the next line*/
			continue;
		}

		/*add 1 to the line number*/
		line_number++;

		/*reset the workable line to the start of the line, and copy the line to the start workable line*/
		workable_line = start_workable_line;
		strcpy(start_workable_line, line);


		/*check if the line is empty or a comment line */
		if (is_empty_line(line) == 0 && is_comment_line(line) == 0) {
			/*extract the first word from the line*/
			workable_line = strtok(workable_line, " \t\n\r");

			/*check if the line is a macro definition(macr)*/
			if (is_line_macro(workable_line) == 1) {
				/*set the current line macro flag to 1, to indicate that the line currently being processed is does macro related line*/
				current_line_macro_flag = 1;
				
				/*if the first word is a macro definition(macr), handle the macro definition line, 
				 * to validate the macro name and add the macro to the list of macros*/
				if (handle_macro_definition_line(line, &macro_exist, start_workable_line, &workable_line, macros,
												 number_of_macros, number_of_rows, error, line_number)) {
					continue;
				}
			}
				/* else if the first word is an ending macro definition(.endmacr)*/
			else if (is_ending_macro(workable_line) == 1) {
				/*set the macro_exist flag to 0*/
				macro_exist = 0;
				
				/* set the current line macro flag to 0, to indicate that the line currently being processed is does macro related line*/
				current_line_macro_flag = 1;
			} else {
				/* set the current line macro flag to 0, to indicate that the line currently being processed is does macro related line*/
				current_line_macro_flag = 1;
				
				/*search for the first word in the macros list*/
				search_result = search_macro_by_name(workable_line, *macros, *number_of_macros);

				/*if the first word is a name of defined macro, add the lines of the macro to the new file*/
				if (search_result != -1) {
					/*if there is no error, add the lines of the macro to the new file*/
					if (error->importance == NO_ERROR) {
						/*realloc more memory to the new file lines array to accommodate the macro lines*/
						new_file = (line_text *) use_realloc(new_file, (number_of_rows +
																		(*macros)[search_result].number_of_macro_lines) *
																	   sizeof(line_text), error);
						/*add the number of macro lines of the macro to the number of rows*/
						number_of_rows += (*macros)[search_result].number_of_macro_lines;

						/*copy the lines of the macro to the new file*/
						for (i = 0; i < (*macros)[search_result].number_of_macro_lines; i++) {
							new_file[number_of_rows - (*macros)[search_result].number_of_macro_lines +
									 i] = (*macros)[search_result].lines[i];
						}
					}
				} else {
					/* set the current line macro flag to 0, to indicate that the line currently being processed is not a macro related line*/
					current_line_macro_flag = 0;
				}
			}

			/* check if the line is a macro related line*/
			if (current_line_macro_flag == 1) {
				/* check if the line is a macro definition line, if it is,
				 * check if there are any text after the macro definition, if there is, print an error and continue to the next line*/
				workable_line = strtok(NULL, "\r\n");
				if (workable_line != NULL && is_empty_line(workable_line) == 0) {
					/*remove the '\n' or '\r' from the end of the line*/
					remove_end_of_line(line);

					/*skip the spaces and tabs in the line*/
					skip_spaces_and_tabs(&workable_line);
					/*print an error message for the extra text after end of the line*/
					print_general_error_no_quoting(EXTRA_TEXT_MESSAGE, EXTRA_TEXT_DESCRIPTION, line, line_number,
												   workable_line - start_workable_line,
												   workable_line - start_workable_line + strlen(workable_line), -1,
												   -1, error);
				}
			}
			
		}

		/*if there is no error, and the current line is not a macro related line, add the line to the new file*/
		if (error->importance == NO_ERROR && current_line_macro_flag == 0) {
			/*if the macro_exist flag is on, add the line to the last macro memory*/
			if (macro_exist == 1) {
				(*macros)[*number_of_macros - 1].number_of_macro_lines++;
				(*macros)[*number_of_macros - 1].lines = (line_text *)use_realloc(
						(*macros)[*number_of_macros - 1].lines,
						(*macros)[*number_of_macros - 1].number_of_macro_lines * sizeof(line_text), error);
				strcpy((*macros)[*number_of_macros - 1].lines[(*macros)[*number_of_macros - 1].number_of_macro_lines -
																1].content, line);
			}
			/*else add the line to the new file*/
			else {
				number_of_rows++;

				/*realloc more memory to the new file lines array to accommodate the new line*/
				new_file = use_realloc(new_file, number_of_rows * sizeof(line_text), error);
				strcpy(new_file[number_of_rows - 1].content, line);
			}
		}
	}

	/*close the file*/
	fclose(file);

	/*if there is no error at all, write the new file to the ".am" file*/
	if (error->importance == NO_ERROR) {
		write_to_file_am(file_name, new_file, number_of_rows, error);
	}

	/*attempt to free the memory allocated*/
	handle_free(new_file);
	free_macros_lines((*macros), *number_of_macros);
}


int handle_macro_definition_line(char line[], int *macro_exist, char *start_workable_line, char **workable_line,
								 macro **macros, int *number_of_macros, int number_of_rows, error *error,
								 int line_number) {
	int macro_valid;

	/*remove the '\n' or '\r' from the end of the line*/
	remove_end_of_line(line);

	/*set the macro_exist flag to 1*/
	*macro_exist = 1;

	/*extract the macro name from the line*/
	*workable_line = strtok(NULL, " \t\n\r");

	/*check if the macro name is valid(macro name is not a saved word and/or not too long)*/
	macro_valid = is_macro_valid(*workable_line, *macros, *number_of_macros, line, line_number, error);

	/*if the macro name is a saved word, switch to the appropriate error message and print it*/
	if (macro_valid != NON_SAVED_WORD) {
		switch (macro_valid) {
			/*print the error message for a too long macro name*/
			case -1:
				print_general_error_no_quoting(ILLEGAL_MACRO_NAME_MESSAGE,
											   MACRO_NAME_IS_TOO_LONG_DESCRIPTION, line, line_number + 1,
											   *workable_line - start_workable_line,
											   *workable_line - start_workable_line + strlen(*workable_line), -1, -1,
											   error);
				break;
				/*print the error message for a macro name that is a saved word opcode.*/
			case SAVED_WORD_OPCODE:
				print_general_error_no_quoting(ILLEGAL_MACRO_NAME_MESSAGE,
											   OPCODE_NAME_AS_MARCO_NAME_DESCRIPTION, line, line_number + 1,
											   *workable_line - start_workable_line,
											   *workable_line - start_workable_line + strlen(*workable_line), -1, -1,
											   error);
				break;
				/*print the error message for a macro name that is a saved word directive.*/
			case SAVED_WORD_DIRECTIVE:
				print_general_error_no_quoting(ILLEGAL_MACRO_NAME_MESSAGE,
											   DIRECTIVE_NAME_AS_MARCO_NAME_DESCRIPTION, line,
											   line_number + 1, *workable_line - start_workable_line,
											   *workable_line - start_workable_line + strlen(*workable_line), -1, -1,
											   error);
				break;
				/*print the error message for a macro name that is a saved word register.*/
			case SAVED_WORD_REGISTER:
				print_general_error_no_quoting(ILLEGAL_MACRO_NAME_MESSAGE,
											   REGISTER_NAME_AS_MARCO_NAME_DESCRIPTION, line,
											   line_number + 1, *workable_line - start_workable_line,
											   *workable_line - start_workable_line + strlen(*workable_line), -1, -1,
											   error);
				break;
				/*print the error message for a macro name that is a saved Word macro.*/
			case SAVED_WORD_MACRO:
				print_general_error_no_quoting(ILLEGAL_MACRO_NAME_MESSAGE,
											   MACRO_NAME_CANNOT_BE_SAVED_WORD_DESCRIPTION, line,
											   line_number + 1, *workable_line - start_workable_line,
											   *workable_line - start_workable_line + strlen(*workable_line), -1, -1,
											   error);
				break;
		}
	}

	/*else if there isn't any error, try to add the macro to the list of macros, if it fails, print an error and continue to the next line*/
	return (error->importance == NO_ERROR &&
			add_macro(*workable_line, macros, number_of_macros, error, number_of_rows) == 0);
}


int search_macro_by_name(const char macro_name[], macro *macros, int number_of_macros) {
	int i;

	/*iterate through the list of macros to find the macro with the specified name*/
	for (i = 0; i < number_of_macros; i++) {
		/*if the current macro name is equal to the given macro name, return the index of the macro*/
		if (strcmp(macro_name, macros[i].macro_name) == 0) {
			return i;
		}
	}

	/*macro not found, return -1*/
	return -1;
}


void free_macros_lines(macro *macros, int number_of_macros) {
	int i;

	/*iterate through the list of macros and free the memory allocated for the lines in each macro*/
	for (i = 0; i < number_of_macros; i++) {
		/*attempt to free the memory allocated*/
		handle_free(macros[i].lines);
	}
}


int add_macro(char macro_name[], macro **macros, int *number_of_macros, error *error, int line_number) {
	/*add 1 to the number of macros*/
	(*number_of_macros)++;

	/*realloc more memory to the macros array to accommodate the new macro, if it fails, return 0*/
	*macros = (macro *) use_realloc(*macros, (*number_of_macros) * sizeof(macro), error);
	if (*macros == NULL) {
		return 0;
	}

	/*fill the new macro fields with the given values*/
	(*macros)[(*number_of_macros) - 1].number_of_macro_lines = 0;
	strcpy((*macros)[(*number_of_macros) - 1].macro_name, macro_name);
	(*macros)[(*number_of_macros) - 1].lines = NULL;
	(*macros)[(*number_of_macros) - 1].line_number = line_number;

	return 1;
}


int is_ending_macro(const char line[]) {
	/*compare the line with the predefined end macro string*/
	return strcmp(line, END_MACRO) == 0;
}

int is_line_macro(const char line[]) {
	/*compare the line with the predefined macro string*/
	return strcmp(line, MACRO) == 0;
}

int is_macro_valid(char segment[], macro *macros, int amount_of_macros, char line[], int line_number, error *error) {
	int found;
	
	/*check if the line exceeds the maximum symbol size, if it does, return -1*/
	if (strlen(segment) > MAX_SYMBOL_SIZE) {
		return -1;
	}

	/* search for the current macro name in the list of macros,
	 * if it is found, print the appropriate error message and return -2*/
	if ((found = search_macro_by_name(segment, macros, amount_of_macros)) != -1) {
		print_macro_already_exists(segment, line, line_number, error, macros[found].line_number + 1);
		return -2;
	}
	
	/*check if the line is a saved word, if it is, return the result of is_a_saved_word(line)*/
	return is_a_saved_word(segment);
}
