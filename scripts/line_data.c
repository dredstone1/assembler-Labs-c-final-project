#include "../header/line_data.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int is_number(char word[]);
int get_number(char word[]);
int is_register(char word[]);

int cast_string_to_words_array(char *string, short **word_array, int length);



int is_register(char word[]) {
    int offset = 0;
    if (word[0] == '*')
        offset = 1;

    if (word[offset] == 'r' && word[offset+1] >= '0' && word[offset+1] <= '7' && word[offset+2] == '\0')
        return 1;

    return 0;
}

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






opcode get_opcode_from_string(const char *str) {
	opcode code;
	for (code = MOV; code <= STOP; code++)
		if (strcmp(str, opcode_names[code][0][0]) == 0)
			return code;

	return -1;
}

int is_valid_var(opcode code, variable_type variable) {
	return opcode_names[code][1][0][variable] != '_';
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

int read_string(char **workable_line, char *line, instruction_data *instruction, error_array *error, int line_number) {
	char *temp_offset;
	int missing_start_quote = 0, missing_end_quote = 0;

	temp_offset = *workable_line;
	if (**workable_line != '\"')
		missing_start_quote = 1;

	(*workable_line) = strchr(++(*workable_line), '\"');
	
	if (*workable_line == NULL)
		missing_end_quote = 1;
	if (missing_start_quote && missing_end_quote)
		add_error(error, MISSING_ENDING_QUOTE_N_START_QUOTE, line_number, strlen(line) - strlen(temp_offset)-1, strlen(line), WARNING, line, strlen(line) - strlen(temp_offset)-1, 0);
	else if (missing_start_quote)
		add_error(error, MISSING_START_QUOTE, line_number, strlen(line) - strlen(temp_offset)-1, strlen(line), WARNING, line, strlen(line) - strlen(temp_offset)-1, 0);
	else if (missing_end_quote)
		add_error(error, MISSING_ENDING_QUOTE, line_number, strlen(line) - strlen(temp_offset), strlen(line), WARNING, line, strlen(line), 0);

	if (error->importance != NO_ERROR)
		return 0;
	
	if (cast_string_to_words_array(temp_offset+1, &(instruction->numbers), *workable_line - temp_offset-1) == 0) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0, 0);
		return 0;
	}

	instruction->numbers[*workable_line - temp_offset - 1] = '\0';
	
	instruction->size = *workable_line - temp_offset;
	(*workable_line)++;
	return 1;
}

int read_data(char **workable_line, char *line, instruction_data *instruction, error_array *error, int line_number) {
	int i, coma_count, number;
	char word1[80];
	instruction->numbers = NULL;
	for (i = 0; i < 76; i++) {
		coma_count = count_commas_until_text(workable_line);
		if (i == 0) {
			if (coma_count != 0) {
				add_error(error, INVALID_COMMA, line_number, *workable_line - line,
						  *workable_line - line + strlen(*workable_line), WARNING, line, 0, 0);
				return 0;
			}
		} else {
			if (coma_count == 0) {
				if (*workable_line - line >= strlen(line))
					break;
				
				add_error(error, MISSING_COMMA, line_number, *workable_line - line, *workable_line - line + strlen(*workable_line), WARNING, line, 0, 0);
				return 0;
			} else if (coma_count > 1) {
				add_error(error, EXTRA_COMMA, line_number, *workable_line - line,
						  *workable_line - line + strlen(*workable_line), WARNING, line, 0, 0);
				return 0;
			}
		}
		
		skip_spaces_and_tabs(workable_line);
		get_next_word(workable_line, word1, " ,\t\0");
		skip_spaces_and_tabs(workable_line);
		
		if ((number = get_number(word1)) == 0) {
			add_error(error, INVALID_NUMBER, line_number, *workable_line - line,
					  *workable_line - line + strlen(*workable_line), WARNING, line, 0, 0);
			return 0;
		}
		instruction->numbers = (short *)use_realloc(instruction->numbers, sizeof(short) * (i + 1), error);

		
		instruction->size = i+1;
		instruction->numbers[i] = number;
	}
	return 1;
}

int cast_string_to_words_array(char *string, short **word_array, int length) {
	*word_array = (short *)malloc(sizeof(short ) * length);
	int i;
	
	if (*word_array == NULL)
		return 0;

	for (i = 0; i < length; i++)
		(*word_array)[i] = string[i];

	return 1;
}

int read_extern_or_entry_symbol(char **workable_line, char *line, instruction_data *instruction, error_array *error, int line_number){
	char *temp_offset = *workable_line;
	*workable_line += strlen(*workable_line)+1;

	skip_spaces_and_tabs(workable_line);
	
	if (!is_valid_symbol_name(*workable_line)){
		add_error(error, INVALID_SYMBOL_NAME, line_number, line - temp_offset,line-*workable_line, WARNING, line, 0, 0);
		return 0;
	}

	instruction->args = *workable_line;
	return 1;
}


int get_next_variable(var *variable, char **workable_line, int comma_needed, error_array *error, int line_number, char* line, opcode type){
	int comma_count;
	char word[80];
	
	skip_spaces_and_tabs(workable_line);
	comma_count = count_commas_until_text(workable_line);

	if (comma_needed){
		if (comma_count == 0) {
			skip_spaces_and_tabs(workable_line);
			get_next_word(workable_line, word, " ,\t\0");

			if (strlen(word) == 0)
				add_error(error, MISSING_PARAMETER_ONE + amount_of_variables_from_opcode(type)-1, line_number, strlen(line) - 1, strlen(line) - 1, WARNING, line,  strlen(line)-1, type);
			else
				add_error(error, MISSING_COMMA, line_number, *workable_line - line, *workable_line - line + strlen(*workable_line), WARNING, line, 0, 0);
		}else if (comma_count > 1)
			add_error(error, EXTRA_COMMA, line_number, *workable_line - line, *workable_line - line + strlen(*workable_line), WARNING, line, 0, 0);
	} else {
		if (comma_count != 0)
			add_error(error, EXTRA_COMMA, line_number, *workable_line - line,
					  *workable_line - line + strlen(*workable_line), WARNING, line, 0, 0);
	}
	if (error->importance != NO_ERROR)
		return 0;

	skip_spaces_and_tabs(workable_line);
	get_next_word(workable_line, word, " ,\t\0");
	if (strlen(word) == 0) {
		add_error(error, MISSING_PARAMETER_ONE + amount_of_variables_from_opcode(type)-1, line_number, strlen(line) - 1, strlen(line) - 1, WARNING, line,  strlen(line)-1, type);
		return 0;
	}
	else if (word[0] == '#') {
		variable->value = get_number(word+1);
		variable->type = IMMEDIATE;
	}
	else if (is_register(word)) {
		if (word[0]=='*') {
			variable->type = REGISTER_INDIRECT;
			word[1] = '0';
		}
		else
			variable->type = REGISTER_DIRECT;

		word[0] = '0';
		variable->value = atoi(word);
	}
	else {
		variable->type = DIRECT;
		variable->var = strdup(word);
	}
	return 1;
}


int read_command_variables(char **workable_line, char *line, command_data *command, error_array *error, int line_number){
	int amount_of_variable;
	
	amount_of_variable = amount_of_variables_from_opcode(command->opcode);
	
	command->source.type = -1;
	command->destination.type = -1;
	
	if (amount_of_variable == 2) {
		int i = strlen(*workable_line);
		*workable_line += strlen(*workable_line)+1;
		

		if (get_next_variable(&command->source, workable_line, 0, error, line_number, line, command->opcode) == 0)
			return 0;
		if (get_next_variable(&command->destination, workable_line, 1, error, line_number, line, command->opcode) == 0)
			return 0;
	} else if (amount_of_variable == 1) {
		*workable_line += strlen(*workable_line)+1;
		
		if (get_next_variable(&command->source, workable_line, 0, error, line_number, line, command->opcode) == 0)
			return 0;
	}
	
	return 1;
}

