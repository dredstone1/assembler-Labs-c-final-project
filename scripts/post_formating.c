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
 * This function checks if the provided line exceeds the maximum symbol size.
 * If it does, the function returns -1. Otherwise, it checks if the line is a saved word.
 * 
 * @param line The string to validate.
 * @return int -1 if the line exceeds the maximum symbol size, otherwise the result of is_a_saved_word(line).
 */
int is_macro_valid(const char line[]);


void post_formating(error *error, char file_name[], macro **macros, int *number_of_macros) {
	line_text *new_file = NULL;
	FILE *file;

	int macro_exist = 0, number_of_rows = 0, search_result, macro_valid, i, line_too_long = 0, line_number = 0;
	char line[MAX_LINE_LENGTH], start_workable_line[MAX_LINE_LENGTH], *workable_line;

	set_ending_to_file_name(file_name, SOURCE_FILE_ENDING);
	file = fopen(file_name, "r");
	if (file == NULL) {
		print_system_error(FILE_NOT_FOUND_MESSAGE);
		error->importance = CRITICAL;
		return;
	}

	while (fgets(line, MAX_LINE_LENGTH, file) != NULL && error->importance != CRITICAL) {
		if (is_line_too_long(line) == 0) {
			if (line_too_long == 0) {
				print_start_line_too_long_error(line_number + 1, error);
			}

			printf("%s", line);
			line_too_long = 1;
			error->importance = WARNING;
			continue;
		} else if (line_too_long == 1) {
			printf("%s", line);
			line_number++;
			print_end_line_too_long_error();
			line_too_long = 0;
			continue;
		}

		line_number++;

		workable_line = start_workable_line;
		strcpy(workable_line, line);

		if (is_empty_line(line) == 0 && is_comment_line(line) == 0) {
			workable_line = strtok(workable_line, " \t\n\r");

			if (is_line_macro(workable_line) == 1) {
				if (line[strlen(line) - 1] == '\n' || line[strlen(line) - 1] == '\r') {
					line[strlen(line) - 1] = '\0';
				}

				macro_exist = 1;
				workable_line = strtok(NULL, " \t\n\r");

				macro_valid = is_macro_valid(workable_line);

				if (macro_valid != NON_SAVED_WORD) {
					switch (macro_valid) {
						case -1:
							print_general_error_no_quoting(ILLEGAL_MACRO_NAME_MESSAGE,
														   MACRO_NAME_IS_TOO_LONG_DESCRIPTION, line, line_number + 1,
														   workable_line - start_workable_line + 1,
														   workable_line - start_workable_line + strlen(workable_line) +
														   1, -1, -1, error);
							break;
						case SAVED_WORD_OPCODE:
							print_general_error_no_quoting(ILLEGAL_MACRO_NAME_MESSAGE,
														   OPCODE_NAME_AS_MARCO_NAME_DESCRIPTION, line, line_number + 1,
														   workable_line - start_workable_line + 1,
														   workable_line - start_workable_line + strlen(workable_line) +
														   1, -1, -1, error);
							break;
						case SAVED_WORD_DIRECTIVE:
							print_general_error_no_quoting(ILLEGAL_MACRO_NAME_MESSAGE,
														   DIRECTIVE_NAME_AS_MARCO_NAME_DESCRIPTION, line,
														   line_number + 1, workable_line - start_workable_line + 1,
														   workable_line - start_workable_line + strlen(workable_line) +
														   1, -1, -1, error);
							break;
						case SAVED_WORD_REGISTER:
							print_general_error_no_quoting(ILLEGAL_MACRO_NAME_MESSAGE,
														   REGISTER_NAME_AS_MARCO_NAME_DESCRIPTION, line,
														   line_number + 1, workable_line - start_workable_line + 1,
														   workable_line - start_workable_line + strlen(workable_line) +
														   1, -1, -1, error);
							break;
						case SAVED_WORD_MACRO:
							print_general_error_no_quoting(ILLEGAL_MACRO_NAME_MESSAGE,
														   MACRO_NAME_CANNOT_BE_SAVED_WORD_DESCRIPTION, line,
														   line_number + 1, workable_line - start_workable_line + 1,
														   workable_line - start_workable_line + strlen(workable_line) +
														   1, -1, -1, error);
							break;
					}
					error->importance = WARNING;
				} else if (error->importance == NO_ERROR &&
						   add_macro(workable_line, macros, number_of_macros, error, number_of_rows) == 0) {
					continue;
				}

				workable_line = strtok(NULL, "\n\r");
				if (workable_line != NULL && is_empty_line(workable_line) == 0) {
					error->importance = WARNING;
					print_general_error_no_quoting(EXTRA_TEXT_MESSAGE, EXTRA_TEXT_DESCRIPTION, line, line_number,
												   workable_line - start_workable_line + 1,
												   workable_line - start_workable_line + strlen(workable_line) + 1, -1,
												   -1, error);
				}

				continue;
			} else if (is_ending_macro(workable_line) == 1) {
				macro_exist = 0;

				workable_line += strlen(END_MACRO) + 1;
				if (workable_line != NULL && is_empty_line(workable_line) == 0) {
					skip_spaces_and_tabs(&workable_line);
					error->importance = WARNING;
					print_general_error_no_quoting(EXTRA_TEXT_MESSAGE, EXTRA_TEXT_DESCRIPTION, line, line_number,
												   workable_line - start_workable_line + 1,
												   workable_line - start_workable_line + strlen(workable_line) - 1, -1,
												   -1, error);
				}
				continue;
			} else if (error->importance == NO_ERROR) {
				search_result = search_macro_by_name(workable_line, *macros, *number_of_macros);

				if (search_result != -1) {
					new_file = (line_text *) use_realloc(new_file, (number_of_rows +
																	(*macros)[search_result].number_of_macro_lines) *
																   sizeof(line_text), error);
					number_of_rows += (*macros)[search_result].number_of_macro_lines;

					for (i = 0; i < (*macros)[search_result].number_of_macro_lines; i++) {
						new_file[number_of_rows - (*macros)[search_result].number_of_macro_lines +
								 i] = (*macros)[search_result].lines[i];
					}

					workable_line = strtok(NULL, "\n\r");
					if (workable_line != NULL && is_empty_line(workable_line) == 0) {
						skip_spaces_and_tabs(&workable_line);
						error->importance = WARNING;
						print_general_error_no_quoting(EXTRA_TEXT_MESSAGE, EXTRA_TEXT_DESCRIPTION, line, line_number,
													   workable_line - start_workable_line + 1,
													   workable_line - start_workable_line + strlen(workable_line) + 1,
													   -1, -1, error);
					}
					continue;
				}
			}
		}

		if (error->importance == NO_ERROR) {
			if (macro_exist == 1) {
				(*macros)[(*number_of_macros) - 1].number_of_macro_lines++;
				(*macros)[(*number_of_macros) - 1].lines = (line_text *) use_realloc(
						(*macros)[(*number_of_macros) - 1].lines,
						(*macros)[*number_of_macros - 1].number_of_macro_lines * sizeof(line_text), error);
				strcpy((*macros)[(*number_of_macros) - 1].lines[(*macros)[*number_of_macros - 1].number_of_macro_lines -
																1].content, line);
			} else {
				number_of_rows++;
				new_file = use_realloc(new_file, (number_of_rows) * sizeof(line_text), error);
				strcpy(new_file[number_of_rows - 1].content, line);
			}
		}
	}


	fclose(file);
	if (error->importance == NO_ERROR) {
		write_to_file_am(file_name, new_file, number_of_rows);
	}
	handle_free(new_file);

	free_macros_lines((*macros), *number_of_macros);
}

int search_macro_by_name(const char macro_name[], macro *macros, int number_of_macros) {
	int i;
	for (i = 0; i < number_of_macros; i++) {
		if (strcmp(macro_name, macros[i].macro_name) == 0) {
			return i;
		}
	}
	return -1;
}

void free_macros_lines(macro *macros, int number_of_macros) {
	int i;
	for (i = 0; i < number_of_macros; i++) {
		handle_free(macros[i].lines);
	}
}

int add_macro(char macro_name[], macro **macros, int *number_of_macros, error *error, int line_number) {
	(*number_of_macros)++;
	*macros = (macro *) use_realloc(*macros, (*number_of_macros) * sizeof(macro), error);
	if (*macros == NULL) {
		return 0;
	}

	(*macros)[(*number_of_macros) - 1].number_of_macro_lines = 0;
	strcpy((*macros)[(*number_of_macros) - 1].macro_name, macro_name);
	(*macros)[(*number_of_macros) - 1].lines = NULL;
	(*macros)[(*number_of_macros) - 1].line_number = line_number;
	return 1;
}

int is_ending_macro(const char line[]) {
	return strcmp(line, END_MACRO) == 0;
}

int is_line_macro(const char line[]) {
	return strcmp(line, MACRO) == 0;
}

int is_macro_valid(const char line[]) {
	if (strlen(line) > MAX_SYMBOL_SIZE) {
		return -1;
	}

	return is_a_saved_word(line);
}
