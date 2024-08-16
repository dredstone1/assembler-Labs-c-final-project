#include "../header/line_data.h"
#include <stdlib.h>
#include <string.h>


/**
 * @brief Checks if a given string represents a valid integer.
 *
 * This function takes a string `word` as input and determines if it 
 * consists only of valid numeric characters (digits 0-9) and an 
 * optional leading sign (+ or -). 
 *
 * @param word The string to be checked for numeric validity.
 * @return 1 if the string is a valid integer, 0 otherwise.
 */
int is_number(char *word);


/**
 * @brief Converts a string representation of a number to an integer.
 *
 * This function attempts to convert the provided string `word` into 
 * an integer. If the string does not represent a valid number, 
 * the function returns 0.
 *
 * @param word The string containing the number to be converted.
 * @return The integer representation of the number if the conversion is 
 *         successful, otherwise 0.
 */
int get_number(char word[]);


/**
 * @brief Converts a string literal to an array of short integers representing ASCII codes.
 *
 * This function takes a string literal `string` and converts it into an array of 
 * `short` integers, where each element stores the ASCII code of the corresponding 
 * character in the string. The resulting array and its size are stored in the 
 * provided `directive_data` structure.
 * 
 * this function will store to convert from the first character to the length character of the string.
 *
 * @param string The input string literal to be converted.
 * @param word_array A pointer to the `directive_data` structure to store the resulting array.
 * @param length The length of the input string.
 * @param error A pointer to an `error` structure to report any allocation errors.
 * @return 1 if the conversion is successful, 0 otherwise.
 */
int cast_string_to_words_array(char *string, directive_data *word_array, int length, error *error);


/**
 * @brief Parses the next variable (operand) from an assembly command.
 * 
 * This function extracts and validates the next operand from the provided
 * `workable_line`. It handles different operand types (immediate, register, 
 * and direct), checks for syntax errors, and ensures the operand is valid for the given opcode.
 * 
 * if finds an error, it will print the appropriate error and return 0.
 * if not, it will store the variable data in the provided `variable` structure and return 1.
 * and will return 1 if the variable is successfully parsed, 0 otherwise.
 * 
 * @param variable A pointer to a `var` structure where the parsed operand data will be stored.
 * @param workable_line A pointer to a pointer to the current position within the line being processed.
 * @param comma_needed A flag indicating whether a comma is required before this operand.
 * @param error A pointer to an `error` structure to report any parsing errors.
 * @param line_number The line number of the current line being processed.
 * @param line The original line of assembly code being processed.
 * @param type The opcode of the current command.
 * @param start_line_pointer A pointer to the beginning of the original line of code.
 * @param usages The operand usage number (0 for first operand, 1 for second, etc.).
 * @return 1 if the variable is successfully parsed, 0 otherwise.
 */
int get_next_variable(var *variable, char **workable_line, int comma_needed, error *error, int line_number, char *line,
					  opcode type, char *start_line_pointer, int usages);


int get_number(char word[]) {
	/*check if the word is a valid number, if not return 0*/
	if (is_number(word) == 0) {
		return 0;
	}

	/*convert the word into a number and return it*/
	return atoi(word);
}


int is_number(char *word) {
	/*move the pointer to the next character if the word starts with a sign*/
	word += *word == '-' || *word == '+';

	/*loop over the word and check if all the characters are digits*/
	while (*word >= '0' && *word <= '9') {
		word++;
	}

	/*return 1 if the word is a number(only digits), 0 otherwise*/
	return *word == '\0';
}


int is_valid_var(opcode code, variable_type variable, int usage) {
	/*check if the variable is a valid variable for the given opcode and usage,
	 * by checking if the variable is not equal to '_' in the opcode_names array*/
	return opcode_names[code][usage + 1][variable] != '_';
}


int amount_of_variables_from_opcode(opcode code) {
	/*return the amount of variables needed for the given opcode
	 * by checking the opcode group and returning the amount of variables needed*/
	if (code < FIRST_GROUP_OPCODE) {
		return 2;
	} else if (code < SECOND_GROUP_OPCODE + FIRST_GROUP_OPCODE) {
		return 1;
	} else if (code < THIRD_GROUP_OPCODE + SECOND_GROUP_OPCODE + FIRST_GROUP_OPCODE) {
		return 0;
	}
	
	/*return 0 if the opcode is not valid*/
	return -1;
}


int is_directive(char *str) {
	/*check if the given string is a directive by checking if it starts with a '.'*/
	return *str == '.';
}


int read_string(char **workable_line, char *line, directive_data *directive, error *error, int line_number,
				char *start_line_pointer) {
	/*temporally pointer to ending quote*/
	char *end_quote;
	/*flags for missing quotes*/
	int missing_start_quote = 0, missing_end_quote = 0;

	/*skip spaces and tabs*/
	skip_spaces_and_tabs(workable_line);

	/*check if the first character is a quote, if not it will set the missing_start_quote flag on*/
	if (**workable_line != '\"') {
		missing_start_quote = 1;
	}
	
	/*move the pointer to the last character of appearance of quote in the line*/
	end_quote = str_last_char(++(*workable_line), '\"');
	
	/*check if the end_quote is NULL, if it is, it will set the missing_end_quote flag on*/
	if (end_quote == NULL) {
		missing_end_quote = 1;
	}
	
	/*check if both flags are on, if they are, it will print the appropriate error, and return 0*/
	if (missing_start_quote && missing_end_quote) {
		print_general_error(MISSING_ENDING_QUOTE_N_START_QUOTE_MESSAGE, MISSING_ENDING_QUOTE_N_START_QUOTE_DESCRIPTION,
							line, line_number, strlen(line) - strlen(*workable_line) - 1, strlen(line) + 1,
							strlen(line) - strlen(*workable_line) - 2, strlen(line), error);
		return 0;
	} 
	/*check if the missing_start_quote flag is on, if it is, it will print the appropriate error, and return 0*/
	else if (missing_start_quote) {
		print_general_error(MISSING_START_QUOTE_MESSAGE, MISSING_START_QUOTE_DESCRIPTION, line, line_number,
							strlen(line) - strlen(*workable_line) - 1, end_quote - start_line_pointer + 1,
							strlen(line) - strlen(*workable_line) - 2, -1,
							error);
		return 0;
	}
	/*check if the missing_end_quote flag is on, if it is, it will print the appropriate error, and return 0*/
	else if (missing_end_quote) {
		print_general_error(MISSING_ENDING_QUOTE_MESSAGE, MISSING_ENDING_QUOTE_DESCRIPTION, line, line_number,
							*workable_line - start_line_pointer - 1, strlen(line) + 1,
							strlen(line), -1, error);
		return 0;
	}
	
	/*cast the string between the quotes into a words array*/
	if (cast_string_to_words_array(*workable_line, directive, end_quote - *workable_line, error) == 0) {
		return 0;
	}
	
	/*set the size of the directive to the length of the string*/
	directive->size = end_quote - *workable_line + 1;
	/*set the last character of the directive to '\0'*/	
	directive->numbers[directive->size - 1] = '\0';
	
	/*move the workable_line pointer to the end of the string*/
	*workable_line = end_quote + 1;
	
	/*the string was successfully read and processed, return 1*/
	return 1;
}


int read_data(char **workable_line, char *line, directive_data *directive, error *error, int line_number,
			  char *start_line_pointer) {
	int i, comma_count, number;
	char word[MAX_LINE_LENGTH], *commas_offset, *temp_offset;

	/* Initialize the directive data structure*/
	directive->numbers = NULL;
	directive->size = 0;
	
	/* Iterate over the line to read the data values*/
	for (i = 0; i < MAX_VARIABLE_DATA_SIZE; i++) {
		commas_offset = *workable_line;
		
		/* Count the number of commas in the line until the next word*/
		comma_count = count_commas_until_text(workable_line);
		
		/*if it is the first iteration*/
		if (i == 0) {
			/*check if there are any commas before the first number, if there are, it will print the appropriate error*/
			if (comma_count != 0) {
				print_general_error(INVALID_COMMA_MESSAGE, INVALID_COMMA_DESCRIPTION, line, line_number,
									*workable_line - start_line_pointer,
									*workable_line - start_line_pointer + strlen(*workable_line), -1, -1, error);
				return 0;
			}
		} 
		/*if it is not the first iteration*/
		else {
			/*if there are no commas before the number, it will print the appropriate error*/
			if (comma_count == 0) {
				print_general_error_no_quoting(MISSING_COMMA_MESSAGE, MISSING_COMMA_DESCRIPTION, line, line_number,
											   commas_offset - start_line_pointer + 1,
											   *workable_line - start_line_pointer + 1, -1, -1, error);
				return 0;

			} 
			/*if there are more than one comma before the number, it will print the appropriate error*/
			else if (comma_count > 1) {
				print_general_error_no_quoting(EXTRA_COMMA_MESSAGE, EXTRA_COMMA_DATA_DESCRIPTION, line, line_number,
											   commas_offset - start_line_pointer + 1,
											   *workable_line - start_line_pointer + 1, -1, -1, error);
				return 0;
			}
		}
		
		temp_offset = *workable_line;
		/*skip spaces and tabs*/
		skip_spaces_and_tabs(workable_line);
		/*get the next word from the line and store it in word*/
		get_next_word(workable_line, word, " ,\t\0");
		
		/*get the number from the word and store it in the number variable and check if the number isn't valid*/
		if ((number = get_number(word)) == 0) {
			/*if the word is empty, it will print the appropriate error*/
			if (strlen(word) == 0) {
				/*if it is the first number, it will print the appropriate error*/
				if (directive->size == 0) {
					print_general_error_no_quoting(MISSING_NUMBER_MESSAGE, MISSING_FIRST_NUMBER_DESCRIPTION, line,
												   line_number, commas_offset - start_line_pointer,
												   commas_offset - start_line_pointer+1,
												   commas_offset - start_line_pointer, -1, error);
				} else {
					print_general_error_no_quoting(MISSING_NUMBER_MESSAGE, MISSING_NUMBER_DESCRIPTION, line,
												   line_number, commas_offset - start_line_pointer,
												   *workable_line - start_line_pointer,
												   *workable_line - start_line_pointer, -1, error);
				}
			}
			/*if the word is not a valid number, it will print the appropriate error*/
			else {
				print_general_error(INVALID_NUMBER_MESSAGE, INVALID_NUMBER_DESCRIPTION, line, line_number,
									temp_offset - start_line_pointer, *workable_line - start_line_pointer, -1,
									-1, error);
			}
			
			/*return 0 to indicate that an error occurred*/
			return 0;
		}
		
		/*check if the number is in the valid range, if it is not, it will print the appropriate error*/
		if (number > DATA_LARGER_NUMBER || number < DATA_SMALLER_NUMBER) {
			print_general_error(NUMBER_OUT_OF_RANGE_MESSAGE, NUMBER_OUT_OF_RANGE_DATA_DESCRIPTION, line, line_number,
								temp_offset - start_line_pointer, *workable_line - start_line_pointer, -1, -1, error);
			return 0;
		}
		
		/*allocate memory for the new number in the directive data structure*/
		directive->numbers = (short *) use_realloc(directive->numbers, sizeof(short) * (i + 1), error);
		
		/*set the size of the directive data to the current index + 1*/
		directive->size = i + 1;
		/*store the number in the directive data structure*/
		directive->numbers[i] = number;
		
		/*check if there are no more characters in the line, if there are no more characters, it will break the loop*/
		if (is_empty_line(*workable_line) == 1) {
			break;
		}
	}
	
	return 1;
}


int cast_string_to_words_array(char *string, directive_data *word_array, int length, error *error) {
	int i;
	
	/*check if the string is NULL, if it is, it will return 0*/
	if (string == NULL) {
		return 0;
	}
	
	/*allocate memory for the numbers in the directive data structure*/
	word_array->numbers = (short *)use_malloc(sizeof(short) * (length + 1), error);
	/*check if the allocation was successful, if it is not, it will return 0*/
	if (word_array->numbers == NULL) {
		return 0;
	}
	
	/*store the string portion in the directive data structure*/
	for (i = 0; i < length; i++) {
		word_array->numbers[i] = string[i];
	}

	return 1;
}


int
read_extern_or_entry_symbol(char **workable_line, char *line, directive_data *directive, error *error, int line_number,
							char *start_workable_line) {
	/*get the next word from the line*/
	*workable_line = strtok(NULL, " \t");
	
	/*check if the word is NULL, if it is, there is no more words in the line, it will print the appropriate error and return 0*/
	if (*workable_line == NULL) {
		if (directive->type == EXTERN) {
			print_general_error_no_quoting(MISSING_SYMBOL_MESSAGE, MISSING_SYMBOL_IN_EXTERN_DESCRIPTION, line,
										   line_number,
										   strlen(line) - 1, strlen(line), strlen(line) - 1, -1, error);
		} else {
			print_general_error_no_quoting(MISSING_SYMBOL_MESSAGE, MISSING_SYMBOL_IN_ENTRY_DESCRIPTION, line,
										   line_number,
										   strlen(line) - 1, strlen(line), strlen(line) - 1, -1, error);
		}
		return 0;
	}
	
	/*check if the word is a valid symbol name, if it is not, it will print the appropriate error and return 0*/
	if (is_valid_symbol_name(*workable_line) == 0) {
		print_general_error(INVALID_SYMBOL_NAME_MESSAGE, INVALID_SYMBOL_NAME_DESCRIPTION, line, line_number,
							strstr(line, *workable_line) - line,
							strstr(line, *workable_line) - line + strlen(*workable_line), -1, -1, error);
		return 0;
	}
	
	/*store the symbol in the directive data structure*/
	strcpy(directive->args, *workable_line);
	
	/*move the workable_line pointer to the end of the word*/
	*workable_line += strlen(*workable_line);
	
	/*replace the first character of the workable_line with the appropriate character in the original line*/
	*workable_line[0] = line[*workable_line - start_workable_line];
	
	return 1;
}


int get_next_variable(var *variable, char **workable_line, int comma_needed, error *error, int line_number, char *line,
					  opcode type, char *start_line_pointer, int usages) {
	int comma_count;
	char word[MAX_LINE_LENGTH], word_temp[MAX_LINE_LENGTH], *temp_offset = *workable_line;
	
	word[0] = '\0';
	
	skip_spaces_and_tabs(workable_line);
	/*count the number of commas in the line until the next word*/
	comma_count = count_commas_until_text(workable_line);

	/*check if the word is empty,
	 * and if it's not (have comma and not needing one) at the same time,
	 * it will print the appropriate missing parameter error and return 0*/
	if (is_empty_line(*workable_line) && (comma_count > 0 && comma_needed == 0) == 0) {
		if (temp_offset == *workable_line) {
			*workable_line += 1;
		} else if (comma_needed == 0) {
			temp_offset -= 1;
		}
		
		if (amount_of_variables_from_opcode(type) == 1) {
			print_general_error_no_quoting(MISSING_PARAMETER_MESSAGE_ONE, MISSING_PARAMETER_DESCRIPTION_ONE, line,
										   line_number, temp_offset - start_line_pointer,
										   *workable_line - start_line_pointer, -1, -1, error);
		} else {
			print_general_error_no_quoting(MISSING_PARAMETER_MESSAGE_TWO, MISSING_PARAMETER_DESCRIPTION_TWO, line,
										   line_number, temp_offset - start_line_pointer,
										   *workable_line - start_line_pointer, -1, -1, error);
		}
		return 0;
	}

	/*check if it needs a comma*/
	if (comma_needed) {
		/*if there are no commas in the line or it the end of the line, 
		 * it's missing a comma, it will print the appropriate error and return 0*/
		if (comma_count == 0 || **workable_line == '\0') {
			print_general_error_no_quoting(MISSING_COMMA_MESSAGE, MISSING_COMMA_DESCRIPTION, line, line_number,
										   temp_offset - start_line_pointer, *workable_line - start_line_pointer, -1, -1,
										   error);
			return 0;
		}
		/*if there are more than one comma in the line, it will print the appropriate extra comma error and return 0*/
		else if (comma_count > 1) {
			print_general_error_no_quoting(EXTRA_COMMA_MESSAGE, EXTRA_COMMA_DESCRIPTION, line, line_number,
										   temp_offset - start_line_pointer,
										   *workable_line - start_line_pointer, -1, -1, error);
			return 0;
		}
	} 
	/*else, comma not needed*/
	else {
		/*if there are commas in the line, it will print the appropriate extra comma error and return 0*/
		if (comma_count != 0) {
			print_general_error_no_quoting(EXTRA_COMMA_MESSAGE, INVALID_COMMA_AFTER_COMMAND_TYPE_DESCRIPTION, line,
										   line_number, temp_offset - start_line_pointer - 1,
										   *workable_line - start_line_pointer, strchr(line, ',') - line, -1,
										   error);
			return 0;
		}
	}

	/*skip spaces and tabs*/
	skip_spaces_and_tabs(workable_line);
	
	/*get the next word from the line and store it in word*/
	get_next_word(workable_line, word, " ,\t");
	
	/*check if the word is having command number starter ('#') at the beginning*/
	if (word[0] == '#') {
		/*check if the word is a valid number, if it is not, it will print the appropriate error and return 0*/
		if (is_number(word + 1) == 0 || strlen(word + 1) == 0) {
			/*if the word is empty, it will print the appropriate missing number error*/
			if (strlen(word + 1) == 0) {
				print_general_error(MISSING_NUMBER_MESSAGE, MISSING_NUMBER_AFTER_NUMBER_STARTER_DESCRIPTION, line,
									line_number,
									strstr(*workable_line, word) - *workable_line,
									*workable_line - start_line_pointer + 1,
									*workable_line - start_line_pointer, -1, error);
			} 
			/*if the word is not a valid number, it will print the appropriate invalid number error*/
			else {
				print_general_error(INVALID_NUMBER_DESCRIPTION, INVALID_NUMBER_DESCRIPTION, line, line_number,
									strstr(temp_offset, word) - start_line_pointer, *workable_line - start_line_pointer,
									-1, -1,
									error);
			}
			return 0;
		}
		
		/*convert the word into a number and store it in the variable data structure*/
		variable->value = get_number(word + 1);
		
		/*check if the number is in the valid range, if it is not, it will print the appropriate out of range error and return 0*/
		if (variable->value > COMMAND_LARGER_NUMBER || variable->value < COMMAND_SMALLER_NUMBER) {
			print_general_error(NUMBER_OUT_OF_RANGE_MESSAGE, NUMBER_OUT_OF_RANGE_COMMAND_DESCRIPTION, line, line_number,
								strstr(temp_offset, word) - start_line_pointer, *workable_line - start_line_pointer, -1,
								-1, error);
			return 0;
		}

		/*set the type of the variable to immediate*/
		variable->type = IMMEDIATE;
	} 
	/*if the word is a valid register*/
	else if (is_register(word)) {
		/*copy the word into the word_temp variable*/	
		strcpy(word_temp, word);
		
		/*check if the word is a register indirect, if it is, it will set the type of the variable to register indirect and remove the '#' from the word_temp
		 * if not, it will set the type of the variable to register direct*/
		if (word_temp[0] == '*') {
			variable->type = REGISTER_INDIRECT;
			word_temp[1] = '0';
		} else {
			variable->type = REGISTER_DIRECT;
		}

		/*replace the 'r' or '*' in the word_temp with '0'*/
		word_temp[0] = '0';

		/*convert the word_temp into a number and store it in the variable data structure*/
		variable->value = atoi(word_temp);
		/*check if the number is in the valid range, if it is not, it will print the appropriate out of range error and return 0*/
		if (variable->value > LAST_REGISTER_COUNT) {
			print_general_error(INVALID_REGISTER_NAME_MESSAGE, INVALID_REGISTER_NAME_DESCRIPTION, line, line_number,
								temp_offset - start_line_pointer, *workable_line - start_line_pointer, -1, -1,
								error);
			return 0;
		}
	}
	/*if the word is not a register or a number, it will be a symbol*/
	else {
		/*check if the symbol name is not too long, if it is, it will print the appropriate symbol too long error and return 0*/
		if (strlen(word) > MAX_SYMBOL_SIZE) {
			print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE, SYMBOL_IS_TOO_LONG_DESCRIPTION, line,
										   line_number, *workable_line - start_line_pointer + 1 - +strlen(word),
										   *workable_line - start_line_pointer + 1, -1, -1, error);
			return 0;
		}
		
		/*set the type of the variable to direct and store the word in the variable data structure*/
		variable->type = DIRECT;
		strcpy(variable->var, word);
	}

	
	/*check if the variable is a valid variable for the given opcode and usage, if it is not, it will print the appropriate error and return 0*/
	if ((is_valid_var(type, variable->type, usages) == 0)) {
		print_command_not_legal(word, line, line_number, error, opcode_names[type][usages + 1],
								!usages, type);
		return 0;
	}
	
	/*return 1 to indicate that no errors occurred*/
	return 1;
}


int read_command_variables(char **workable_line, char *line, command_data *command, error *error, int line_number,
						   char *start_workable_line) {
	int amount_of_variable, commas;
	char *workable_line_temp;

	/*get the amount of variables needed for the given opcode*/
	amount_of_variable = amount_of_variables_from_opcode(command->opcode);
	
	/*initialize the source and destination variables*/
	command->source.type = -1;
	command->destination.type = -1;
	
	/*check if the amount of variables is 2*/
	if (amount_of_variable == 2) {
		/*store the current position of the workable_line for later use in case of an error*/
		workable_line_temp = *workable_line;
		/*get the next word(the next string until the end of the line) from the line*/
		*workable_line = strtok(NULL, "");
		/*if the word is NULL, there are no more words in the line, it will set the workable_line to the end of the line*/
		if (*workable_line == NULL) {
			*workable_line = workable_line_temp + strlen(workable_line_temp);
		}
		
		/*get the next variable from the line and store it in the source variable
		 * if the function returns 0, it will return 0*/
		if (get_next_variable(&(command->source), workable_line, 0, error, line_number, line, command->opcode,
							  start_workable_line, 1) == 0) {
			return 0;
		}
		/*get the next variable from the line and store it in the destination variable
		 * if the function returns 0, it will return 0*/
		if (get_next_variable(&(command->destination), workable_line, 1, error, line_number, line, command->opcode,
							  start_workable_line, 0) == 0) {
			return 0;
		}
	}
	/*check if the amount of variables is 1*/
	else if (amount_of_variable == 1) {
		/*store the current position of the workable_line for later use in case of an error*/
		workable_line_temp = *workable_line;
		/*get the next word(the next string until the end of the line) from the line*/
		*workable_line = strtok(NULL, "");
		/*if the word is NULL, there are no more words in the line, it will set the workable_line to the end of the line*/
		if (*workable_line == NULL) {
			*workable_line = workable_line_temp + strlen(workable_line_temp);
		}
		
		/*get the next variable from the line and store it in the destination variable
		 * if the function returns 0, it will return 0*/
		if (get_next_variable(&(command->destination), workable_line, 0, error, line_number, line, command->opcode,
							  start_workable_line, 0) == 0) {
			return 0;
		}
	
		/* check if there are any more characters in the line, that can be a variable, if there are, it will print the appropriate error*/
		workable_line_temp = *workable_line;
		commas = count_commas_until_text(&workable_line_temp);
		skip_spaces_and_tabs(&workable_line_temp);
		if (commas > 0 && *workable_line_temp != '\0') {
			print_general_error_no_quoting(EXTRA_PARAMETER_MESSAGE, EXTRA_PARAMETER_DESCRIPTION, line, line_number,
										   *workable_line - start_workable_line,
										   *workable_line - start_workable_line + strlen(*workable_line), -1, -1,
										   error);
			return 0;
		}
	}
	
	/*no errors occurred, return 1*/
	return 1;
}