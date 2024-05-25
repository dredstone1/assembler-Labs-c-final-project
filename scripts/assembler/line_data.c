#include "line_data.h"
#include "directive.h"
#include <stdlib.h>
#include <string.h>

line_command* line_command_set(int offset, char line[], char first_word[]);
line_directive* line_directive_set(int offset, char line[], char first_word[]);
void handle_variables_command(int offset, char line[], line_command *command);
variable get_next_variable(int *offset, char line[]);
bool is_number(char word[]);
int get_number(char word[]);
bool is_register(char word[]);

void line_data_set(line_data *data, int offset, char line[]){
    char word[LINE_SIZE];
    get_next_word_n_skip(word, &offset, line, " \t\0", 3,FALSE);

    if (word[0] == '.') {
        data->directive = line_directive_set(offset, line, word);
    }
    else{
        data->command = line_command_set(offset, line, word);
    }
}

line_directive* line_directive_set(int offset, char line[], char first_word[]){
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

    return directive;
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
    int amount_of_variable = amount_of_variables_from_opcode(command->opcode), i;

    for (i=0; i<amount_of_variable; i++){
        command->variables[i] = get_next_variable(&offset, line);
    }
}

variable get_next_variable(int *offset, char line[]){
    variable variable;
    char word[LINE_SIZE];

    get_next_word(word, offset, line, " ,\t\0", 5, TRUE);

    if (word[0] == '#') {
        word[0] = 0;
        variable.value = get_number(word);
        variable.type = IMMEDIATE;
    }
    if (is_register(word)) {
        if (word[0]=='*') {
            variable.type = REGISTER_INDIRECT;
        }
        else {
            variable.type = REGISTER_DIRECT;
        }
        word[0] = '0';
        word[1] = '0';
        variable.value = atoi(word);
    }
    else {
        variable.type = DIRECT;
        strcpy(variable.symbol, word);
    }
    return variable;
}

bool is_register(char word[]) {
    if (word[0] == 'r' && word[1] >= '0' && word[1] <= '7' && word[2] == '\0')
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


bool is_separator_between(char line[], int *offset, char separator){
    skip_spaces_and_tabs(offset, line);
    if (line[*offset] == separator){
        skip_spaces_and_tabs(offset, line);
        return TRUE;
    }

    return FALSE;
}
