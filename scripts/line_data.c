#include "../header/line_data.h"
#include <stdlib.h>
#include <string.h>

int is_number(char word[]);
int get_number(char word[]);


int cast_string_to_words_array(char *string, instruction_data *word_array, int length, error *error);

int get_number(char word[]){
    if (is_number(word)==0)
        return 0;

    return atoi(word);
}


int is_number(char word[]){
    int i;
    for (i=0; word[i] != '\0'; i++){
        if ((word[i] >= '0' && word[i] <= '9') || word[i] == '-'|| word[i] == '+')
            continue;
        return 0;
    }

    return 1;
}


int is_valid_var(opcode code, variable_type variable, int usage) {
	return opcode_names[code][usage+1][variable] != '_';
}


int amount_of_variables_from_opcode(opcode code) {
	if (code < FIRST_GROUP_OPCODE)
		return 2;
	else if (code < SECOND_GROUP_OPCODE+FIRST_GROUP_OPCODE)
		return 1;
	else if (code < THIRD_GROUP_OPCODE+SECOND_GROUP_OPCODE+FIRST_GROUP_OPCODE)
		return 0;
	return -1;
}


int is_directive(char *str) {
	return *str == '.';
}

int read_string(char **workable_line, char *line, instruction_data *instruction, error *error, int line_number) {
	char *temp_offset;
	int missing_start_quote = 0, missing_end_quote = 0;
	temp_offset = *workable_line;

	if (**workable_line != '\"') {
		missing_start_quote = 1;
	}

	(*workable_line) = strchr(++(*workable_line), '\"');

	if (*workable_line == NULL) {
		missing_end_quote = 1;
	}
	if (missing_start_quote && missing_end_quote){
		print_general_error(MISSING_ENDING_QUOTE_N_START_QUOTE_MESSAGE, MISSING_ENDING_QUOTE_N_START_QUOTE_DESCRIPTION,
							line, line_number, strlen(line) - strlen(temp_offset), strlen(line) + 1,
							strlen(line) - strlen(temp_offset), strlen(line) + 1, error);
		error->importance = WARNING;
	}else if (missing_start_quote) {
		print_general_error(MISSING_START_QUOTE_MESSAGE, MISSING_START_QUOTE_DESCRIPTION, line, line_number,
							strlen(line) - strlen(temp_offset), strlen(line), strlen(line) - strlen(temp_offset), 0,
							error);
		error->importance = WARNING;
	}else if (missing_end_quote) {
		print_general_error(MISSING_ENDING_QUOTE_MESSAGE, MISSING_ENDING_QUOTE_DESCRIPTION, line, line_number,
							strlen(line) - strlen(temp_offset) + 1, strlen(line) + 1,
							strlen(line) + 1, 0, error);
		error->importance = WARNING;
	}
	if (error->importance != NO_ERROR)
		return 0;

	if (cast_string_to_words_array(temp_offset + 1, instruction, *workable_line - temp_offset - 1, error) == 0)
		return 0;

	instruction->size = *workable_line - temp_offset;

	instruction->numbers[*workable_line - temp_offset - 1] = '\0';

	(*workable_line)++;

	return 1;
}

int read_data(char **workable_line, char *line, instruction_data *instruction, error *error, int line_number, char *start_line_pointer) {
	int i, coma_count, number;
	char word1[80], *temp_offset;
	instruction->numbers = NULL;
	for (i = 0; i < 76; i++) {
		temp_offset = *workable_line;
		coma_count = count_commas_until_text(workable_line);
		
		if (i == 0) {
			if (coma_count != 0) {
				print_general_error(INVALID_COMMA_MESSAGE, INVALID_COMMA_DESCRIPTION, line, line_number,
									*workable_line - start_line_pointer,
									*workable_line - start_line_pointer + strlen(*workable_line), 0, 0, error);
				error->importance = WARNING;
				return 0;
			}
		} else {
			if (coma_count == 0) {
				error->importance = WARNING;
				print_general_error_no_quoting(MISSING_COMMA_MESSAGE, MISSING_COMMA_DESCRIPTION, line, line_number,
											   temp_offset - start_line_pointer + 1,
											   *workable_line - start_line_pointer + 1, 0, 0, error);
				return 0;
				
			} else if (coma_count > 1) {
				print_general_error_no_quoting(EXTRA_COMMA_MESSAGE, EXTRA_COMMA_DATA_DESCRIPTION, line, line_number,
											   temp_offset - start_line_pointer + 1,
											   *workable_line - start_line_pointer + 1, 0, 0, error);
				error->importance = WARNING;
				return 0;
			}
		}
		
		skip_spaces_and_tabs(workable_line);
		get_next_word(workable_line, word1, " ,\t\0");

		if ((number = get_number(word1)) == 0) {
			print_general_error(INVALID_VARIABLE_TYPE_MESSAGE, INVALID_VARIABLE_TYPE_DESCRIPTION, line, line_number,
								*workable_line - line, *workable_line - line + strlen(*workable_line), 0, 0, error);
			error->importance = WARNING;
			return 0;
		}
		
		instruction->numbers = (short *)use_realloc(instruction->numbers, sizeof(short) * (i + 1), error);

		
		instruction->size = i+1;
		instruction->numbers[i] = number;
		if(**workable_line == '\0')
			break;
	}
	return 1;
}

int cast_string_to_words_array(char *string, instruction_data *word_array, int length, error *error) {
	int i;
	word_array->numbers = (short *)use_malloc(sizeof(short) * (length+1), error);

	if (word_array->numbers == NULL)
		return 0;

	for (i = 0; i < length; i++)
		(word_array->numbers)[i] = string[i];
	
	return 1;
}

int read_extern_or_entry_symbol(char **workable_line, char *line, instruction_data *instruction, error *error, int line_number){
	char *end_of_symbol;
	*workable_line += strlen(*workable_line)+1;
	
	skip_spaces_and_tabs(workable_line);
	end_of_symbol = search_for_next_white_char(*workable_line);
	if (*end_of_symbol != '\n' && *end_of_symbol != '\r' && *end_of_symbol != '\0'){
		if (extra_char_at_end(end_of_symbol) == 1){
			print_general_error_no_quoting(EXTRA_TEXT_MESSAGE, EXTRA_TEXT_DESCRIPTION, line, line_number, *workable_line - line + 1, *workable_line - line + strlen(*workable_line), 0, 0, error);
			error->importance = WARNING;
			return 0;
		}
		*end_of_symbol = '\0';
	}
	if (is_valid_symbol_name(*workable_line) == 0){
		print_general_error(INVALID_SYMBOL_NAME_MESSAGE, INVALID_SYMBOL_NAME_DESCRIPTION, line, line_number,
							strstr(line, *workable_line) - line + 1,
							strstr(line, *workable_line) - line + strlen(*workable_line) + 1, 0, 0, error);
		error->importance = WARNING;
		
		return 0;
	}
	strcpy(instruction->args, *workable_line);
	
	return 1;
}


int get_next_variable(var *variable, char **workable_line, int comma_needed, error *error, int line_number, char* line, opcode type, char *start_line_pointer, int usages){
	int comma_count;
	char word[80], word_temp[80], *temp_offset = *workable_line, *commas_offset;

	skip_spaces_and_tabs(workable_line);
	comma_count = count_commas_until_text(workable_line);
	
	if (comma_needed) {
		if (comma_count == 0) {
			commas_offset = *workable_line;
			skip_spaces_and_tabs(workable_line);
			get_next_word(workable_line, word, " ,\t\0");
			
			if (strlen(word) == 0) {
				if (temp_offset == *workable_line)
					*workable_line += 1;
				if (amount_of_variables_from_opcode(type) == 1) {
					print_general_error_no_quoting(MISSING_PARAMETER_MESSAGE_ONE, MISSING_PARAMETER_DESCRIPTION_ONE, line, line_number, temp_offset - start_line_pointer + 1, *workable_line - start_line_pointer + 1, 0, 0, error);
					error->importance = WARNING;
				}else {
					print_general_error_no_quoting(MISSING_PARAMETER_MESSAGE_TWO, MISSING_PARAMETER_DESCRIPTION_TWO, line, line_number, temp_offset - start_line_pointer + 1, *workable_line - start_line_pointer + 1, 0, 0, error);
					error->importance = WARNING;
				}
			} else {
				error->importance = WARNING;
				print_general_error_no_quoting(MISSING_COMMA_MESSAGE, MISSING_COMMA_DESCRIPTION, line, line_number, temp_offset - start_line_pointer + 1, commas_offset - start_line_pointer + 1, 0, 0, error);
			}
		} else if (comma_count > 1) {
			error->importance = WARNING;
			print_general_error_no_quoting(EXTRA_COMMA_MESSAGE, EXTRA_COMMA_DESCRIPTION, line, line_number, temp_offset - start_line_pointer + 1, *workable_line - start_line_pointer + 1, 0, 0, error);
		}
	} else {
		if (comma_count != 0) {
			print_general_error_no_quoting(EXTRA_COMMA_MESSAGE, INVALID_COMMA_AFTER_COMMAND_TYPE_DESCRIPTION, line, line_number, temp_offset - start_line_pointer, *workable_line - start_line_pointer, strchr(line, ',')-line+1, 0, error);
			error->importance = WARNING;
		}
	}
	if (error->importance != NO_ERROR)
		return 0;

	skip_spaces_and_tabs(workable_line);
	get_next_word(workable_line, word, " ,\t");
	if (strlen(word) == 0) {
		if (strlen(word) == 0) {
			if (amount_of_variables_from_opcode(type) == 1) {
				print_general_error_no_quoting(MISSING_PARAMETER_MESSAGE_ONE, MISSING_PARAMETER_DESCRIPTION_ONE, line, line_number, temp_offset - start_line_pointer + 1, *workable_line - start_line_pointer + 1, 0, 0, error);
				error->importance = WARNING;
			}else {
				print_general_error_no_quoting(MISSING_PARAMETER_MESSAGE_TWO, MISSING_PARAMETER_DESCRIPTION_TWO, line, line_number, temp_offset - start_line_pointer + 1, *workable_line - start_line_pointer + 1, 0, 0, error);
				error->importance = WARNING;
			}
		}
		
		return 0;
	}
	else if (word[0] == '#') {
		if (is_number(word+1) == 0 || strlen(word+1)==0){
			print_general_error(INVALID_NUMBER_MESSAGE, INVALID_NUMBER_DESCRIPTION, line, line_number, temp_offset - start_line_pointer + 1, *workable_line - start_line_pointer + 1, 0, 0, error);
			error->importance = WARNING;
			return 0;
		}
		variable->value = get_number(word+1);
		variable->type = IMMEDIATE;
			
	}
	else if (is_register(word)) {
		strcpy(word_temp, word);
		if (word_temp[0]=='*') {
			variable->type = REGISTER_INDIRECT;
			word_temp[1] = '0';
		}
		else
			variable->type = REGISTER_DIRECT;

		word_temp[0] = '0';
		
		variable->value = atoi(word_temp);
		if (variable->value > 7){
			print_general_error(INVALID_REGISTER_NAME_MESSAGE, INVALID_REGISTER_NAME_DESCRIPTION, line, line_number, temp_offset - start_line_pointer + 1, *workable_line - start_line_pointer + 1, 0, 0, error);
			error->importance = WARNING;
			return 0;
		}	
	}
	else {
		if (strlen(word) > MAX_SYMBOL_SIZE) {
			print_general_error_no_quoting(INVALID_SYMBOL_NAME_MESSAGE, SYMBOL_IS_TOO_LONG_DESCRIPTION, line, line_number, *workable_line - start_line_pointer + 1-+ strlen(word), *workable_line - start_line_pointer + 1, 0, 0, error);
			error->importance = WARNING;
			return 0;
		}
		variable->type = DIRECT;
		strcpy(variable->var, word);
	}
	
	if (is_valid_var(type, variable->type, usages) == 0) {
		print_command_not_legal(word, line, line_number, error, opcode_names[type][usages+1], usages, type);
		error->importance = WARNING;
		return 0;
	}
	return 1;
}


int read_command_variables(char **workable_line, char *line, command_data *command, error *error, int line_number, char *start_workable_line){
	int amount_of_variable, commas;
	char *workable_line_temp;
	
	amount_of_variable = amount_of_variables_from_opcode(command->opcode);
	
	command->source.type = -1;
	command->destination.type = -1;
	
	if (amount_of_variable == 2) {
		*workable_line += strlen(*workable_line)+1;
		
		if (get_next_variable(&command->source, workable_line, 0, error, line_number, line, command->opcode, start_workable_line, 1) == 0)
			return 0;
		if (get_next_variable(&command->destination, workable_line, 1, error, line_number, line, command->opcode, start_workable_line, 0) == 0)
			return 0;
	} else if (amount_of_variable == 1) {
		workable_line_temp = *workable_line;
		*workable_line = strtok(NULL, " ,\t");
		if (*workable_line == NULL){
			
			print_general_error_no_quoting(MISSING_PARAMETER_MESSAGE_ONE, MISSING_PARAMETER_DESCRIPTION_ONE, line, line_number, workable_line_temp - start_workable_line +
					strlen(workable_line_temp)+1, workable_line_temp - start_workable_line + strlen(workable_line_temp) + 1, workable_line_temp - start_workable_line + strlen(workable_line_temp) + 1, 0, error);
			error->importance = WARNING;
			return 0;
		}

		if (get_next_variable(&command->destination, workable_line, 0, error, line_number, line, command->opcode, start_workable_line, 0) == 0)
			return 0;
		
		workable_line_temp = *workable_line;
		commas = count_commas_until_text(&workable_line_temp);
		skip_spaces_and_tabs(&workable_line_temp);
		if (commas > 0 && *workable_line_temp != '\0') {
			print_general_error_no_quoting(EXTRA_PARAMETER_MESSAGE, EXTRA_PARAMETER_DESCRIPTION, line, line_number, *workable_line - start_workable_line+1, *workable_line - start_workable_line+1 + strlen(*workable_line), 0, 0, error);
			error->importance = WARNING;
			return 0;
		}
	}
	
	return 1;
}
