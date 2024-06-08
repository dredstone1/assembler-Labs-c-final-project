#include "line_data.h"
#include "directive.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

line_command* line_command_set(int offset, char line[], char first_word[]);
line_directive* line_directive_set(int offset, char line[], char first_word[], symbol symbol[]);
void handle_variables_command(int offset, char line[], line_command *command);
variable get_next_variable(int *offset, char line[]);
bool is_number(char word[]);
int get_number(char word[]);
bool is_register(char word[]);
void handle_variables_directive(int offset, char line[], line_directive *directive, symbol symbol[]);
void handle_variables_string(char line[], int *offset, int variables[], int *amount_of_variables);
void handle_variables_data(char line[], int *offset, int variables[], int *amount_of_variables);
void set_variables_list(int offset, char line[], line_directive *directive);
void cast_string_to_int_array(char string[], int int_array[], int *length);

void line_data_set(line_data *data, int offset, char line[], symbol symbol[]){
    char word[LINE_SIZE];
    get_next_word_n_skip(word, &offset, line, " \t\0", 3);

    if (word[0] == '.')
        data->directive = line_directive_set(offset, line, word, symbol);
    else
        data->command = line_command_set(offset, line, word);
}

line_directive* line_directive_set(int offset, char line[], char first_word[], symbol symbol[]){
    char word[LINE_SIZE];
    strcpy(word, first_word);

    line_directive *directive = (line_directive*)malloc(sizeof(line_directive));
    if (directive == NULL) {
        /*error, memory allocation failed*/
        return NULL;
    }

    directive->type = get_directive_from_string(word);
    if (directive->type == -1) {
        /*error, command not found*/
        return NULL;
    }
    handle_variables_directive(offset, line, directive, symbol);

    return directive;
}

void handle_variables_directive(int offset, char line[], line_directive *directive, symbol *symbol){
    variable variable_temp;
    if (directive->type == ENTRY_ || directive->type == EXTERNAL) {
        if (symbol->label[0] != '\0') {
            /*contain start_symbol in external or entry line error*/
            return;
        }
        variable_temp = get_next_variable(&offset, line);
        strcpy(symbol->label, variable_temp.symbol);
        symbol->type = (symbol_type)directive->type;
    }
    else
        set_variables_list(offset, line, directive);
}

void set_variables_list(int offset, char line[], line_directive *directive){
    directive->amount_of_variables = 0;
    if (directive->type == DATA)
        handle_variables_data(line, &offset, directive->variables, &directive->amount_of_variables);
    else
        handle_variables_string(line, &offset, directive->variables, &directive->amount_of_variables);
}

void handle_variables_data(char line[], int *offset, int variables[], int *amount_of_variables){
    int i, coma_count;
    char word[LINE_SIZE];

    for (i=0; i<MAX_LIST_SIZE; i++){
        coma_count = count_char_until_not_separator(line, ',', offset, " ,\t\0", 4);

        if (i==0){
            if (coma_count != 0) {
                /*invalid comma error*/
            }
        }
        else{
            if (coma_count == 0) {
                /*missing comma error*/
            } else if (coma_count > 1) {
                /*extra comma error*/
            }
        }

        get_next_word_n_skip(word, offset, line, " ,\t\0", 4);
        variables[i] = get_number(word);
        if (variables[i] == 0) {
            break;
        }
        (*amount_of_variables)++;
    }
    printf("amount of variables: %d\n", *amount_of_variables);
}

void handle_variables_string(char line[], int *offset, int variables[], int *amount_of_variables){
    char word[LINE_SIZE];
    skip_spaces_and_tabs(offset, line);
    if (line[*offset] != '\"') {
        /*missing " error*/
    }
    (*offset)++;
    get_next_word(word, offset, line, "\"\0", 2);
    if (line[*offset] != '\"') {
        /*missing " error*/
    }
    cast_string_to_int_array(word, variables, amount_of_variables);
}

void cast_string_to_int_array(char string[], int int_array[], int *length){
    do
        int_array[*length] = string[*length];
    while (string[(*length)++] != '\0');
}


line_command* line_command_set(int offset, char line[], char first_word[]) {
    char word[LINE_SIZE];
    strcpy(word, first_word);

    line_command *command = (line_command*)malloc(sizeof(line_command));
    if (command == NULL) {
        /*error, memory allocation failed*/
        return NULL;
    }

    command->opcode = get_opcode_from_string(word);
    if (command->opcode == -1) {
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
