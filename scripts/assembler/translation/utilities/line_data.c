#include "line_data.h"
#include "directive.h"
#include <stdlib.h>
#include <string.h>

line_command* line_command_set(int offset, char line[], char first_word[], error_array *error, int line_number);
line_directive* line_directive_set(int offset, char line[], char first_word[], symbol symbol[], error_array *error, int line_number);
void handle_variables_command(int offset, char line[], line_command *command);
variable get_next_variable(int *offset, char line[]);
bool is_number(char word[]);
int get_number(char word[]);
bool is_register(char word[]);
void handle_variables_directive(int offset, char line[], line_directive *directive, symbol *symbol, error_array *error, int line_number);
void handle_variables_string(char line[], int *offset, int variables[], int *amount_of_variables, error_array *error, int line_number);
void handle_variables_data(char line[], int *offset, int variables[], int *amount_of_variables, error_array *error, int line_number);
void set_variables_list(int offset, char line[], line_directive *directive, error_array *error, int line_number);
void cast_string_to_int_array(char string[], int int_array[], int *length);

void line_data_set(line_data *data, int offset, char line[], symbol symbol[], error_array *error, int line_number){
    char word[LINE_SIZE];
    get_next_word_n_skip(word, &offset, line, " \t\0", 3);

    if (word[0] == '.')
        data->directive = line_directive_set(offset, line, word, symbol, error, line_number);
    else
        data->command = line_command_set(offset, line, word, error, line_number);
}

line_directive* line_directive_set(int offset, char line[], char first_word[], symbol symbol[], error_array *error, int line_number){
    char word[LINE_SIZE];
    strcpy(word, first_word);

    line_directive *directive = (line_directive*)malloc(sizeof(line_directive));
    if (directive == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
        return NULL;
    }

    directive->type = get_directive_from_string(word);
    if (directive->type == -1) {
		add_error(error, DIRECTIVE_TYPE_MISSING, line_number, offset, strlen(word), WARNING, line, offset);
        return NULL;
    }
    handle_variables_directive(offset, line, directive, symbol, error, line_number);

    return directive;
}

void handle_variables_directive(int offset, char line[], line_directive *directive, symbol *symbol, error_array *error, int line_number) {
    variable variable_temp;
    if (directive->type == ENTRY_ || directive->type == EXTERNAL) {
        if (symbol->label[0] != '\0') {
			add_error(error, SYMBOL_IN_EXTERNAL_OR_ENTRY, line_number, 0, strlen(symbol->label), WARNING,
					  line, 0);
            return;
        }
        variable_temp = get_next_variable(&offset, line);
        strcpy(symbol->label, variable_temp.symbol);
        symbol->type = (symbol_type)directive->type;
    }
    else
        set_variables_list(offset, line, directive, error, line_number);
}

void set_variables_list(int offset, char line[], line_directive *directive, error_array *error, int line_number){
    directive->amount_of_variables = 0;
    if (directive->type == DATA)
        handle_variables_data(line, &offset, directive->variables, &directive->amount_of_variables, error, line_number);
    else
        handle_variables_string(line, &offset, directive->variables, &directive->amount_of_variables, error, line_number);
}

void handle_variables_data(char line[], int *offset, int variables[], int *amount_of_variables, error_array *error, int line_number){
    int i, coma_count, temp_offset;
    char word[LINE_SIZE];

    for (i=0; i<MAX_LIST_SIZE; i++){
        temp_offset = *offset;
        coma_count = count_char_until_not_separator(line, ',', offset, " ,\t\0", 4);

        if (i==0){
            if (coma_count != 0) {
				add_error(error, INVALID_COMMA, line_number, temp_offset, *offset, WARNING, line, 0);
                return;
            }
        }
        else{
            if (coma_count == 0) {
				add_error(error, MISSING_COMMA, line_number, temp_offset, *offset, WARNING, line, 0);
                return;
            } else if (coma_count > 1) {
				add_error(error, EXTRA_COMMA, line_number, temp_offset, *offset, WARNING, line, 0);
                return;
            }
        }
        if (error->importance != NONE)
            return;

        get_next_word_n_skip(word, offset, line, " ,\t\0", 4);
        variables[i] = get_number(word);
        if (variables[i] == 0)
            break;
        (*amount_of_variables)++;
    }
}

void handle_variables_string(char line[], int *offset, int variables[], int *amount_of_variables, error_array *error, int line_number){
    char word[LINE_SIZE], temp_offset;

    skip_spaces_and_tabs(offset, line);
    temp_offset = *offset;
    if (line[*offset] != '\"') {
		add_error(error, MISSING_START_QUOTE, line_number, temp_offset - 1, strlen(line), WARNING, line,
				  temp_offset - 1);
        return;
    }
    (*offset)++;
    get_next_word(word, offset, line, "\"\0", 2);
    if (line[*offset] != '\"') {
		add_error(error, MISSING_ENDING_QUOTE, line_number, temp_offset, strlen(line), WARNING, line, (*offset));
        return;
    }
    cast_string_to_int_array(word, variables, amount_of_variables);
}

void cast_string_to_int_array(char string[], int int_array[], int *length){
    do
        int_array[*length] = string[*length];
    while (string[(*length)++] != '\0');
}


line_command* line_command_set(int offset, char line[], char first_word[], error_array *error, int line_number) {
    char word[LINE_SIZE];
    line_command *command = (line_command*)malloc(sizeof(line_command));
    if (command == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
        return NULL;
    }
    
    strcpy(word, first_word);

    command->opcode = get_opcode_from_string(word);
    if (command->opcode == -1) {
		add_error(error, INVALID_OPCODE, line_number, offset - strlen(word) - 1, offset - 1, WARNING, line, 0);
        /*error, command not found*/
        return NULL;
    }

    handle_variables_command(offset, line, command);

    return command;
}

void handle_variables_command(int offset, char line[], line_command *command){
    int amount_of_variable;
    command->variables[0].type = command->variables[1].type = NONE;

    amount_of_variable=amount_of_variables_from_opcode(command->opcode);

    if (amount_of_variable == 2) {
        count_char_until_not_separator(line, ',', &offset, " ,\t\0", 4);
        command->variables[1] = get_next_variable(&offset, line);
        count_char_until_not_separator(line, ',', &offset, " ,\t\0", 4);
        command->variables[0] = get_next_variable(&offset, line);
    }else if (amount_of_variable == 1)
        command->variables[0] = get_next_variable(&offset, line);
}

variable get_next_variable(int *offset, char line[]){
    variable variable;
    char word[LINE_SIZE];
    get_next_word_n_skip(word, offset, line, " ,\t\0", 4);

    if (word[0] == '#') {
        variable.value = get_number(word+1);
        variable.type = IMMEDIATE;
    }
    else if (is_register(word)) {
        if (word[0]=='*') {
            variable.type = REGISTER_INDIRECT;
            word[1] = '0';
        }
        else
            variable.type = REGISTER_DIRECT;

        word[0] = '0';
        variable.value = atoi(word);
    }
    else {
        variable.type = DIRECT;
        strcpy(variable.symbol, word);
    }
    return variable;
}

bool is_register(char word[]) {
    int offset = 0;
    if (word[0] == '*')
        offset = 1;

    if (word[offset] == 'r' && word[offset+1] >= '0' && word[offset+1] <= '7' && word[offset+2] == '\0')
        return TRUE;

    return FALSE;
}

int get_number(char word[]){
    if (is_number(word)==FALSE)
        return 0;

    return atoi(word);
}


bool is_number(char word[]){
    int i;
    for (i=0; word[i] != '\0'; i++){
        if ((word[i] >= '0' && word[i] <= '9') || word[i] == '-'|| word[i] == '+')
            continue;
        return FALSE;
    }

    return TRUE;
}
