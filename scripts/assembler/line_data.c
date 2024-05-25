#include "line_data.h"
#include "directive.h"
#include <stdlib.h>
#include <string.h>

line_command* line_command_set(int offset, char line[], char first_word[]);
line_directive* line_directive_set(int offset, char line[], char first_word[]);
void handle_variables_command(int offset, char line[], line_command *command);
int get_next_variable(int *offset, char line[]);

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
    int variable;
    char word[LINE_SIZE];
    get_next_word(word, offset, line, " ,\t\0", 5, TRUE);
    get_variable_from_string(word);
}

variable get_variable_from_string(char word[]) {
    if (word[0] == '#') {

    }
    if (word[0] == '*') {

    }
}

int get_register_code_from_string(char word[]){
    if (word[0] == 'r' && word[1] >= '0' && word[1] <= '7') {
        return word[1] - '0';
    }

    return -1;
}

bool is_separator_between(char line[], int *offset, char separator){
    skip_spaces_and_tabs(offset, line);
    if (line[*offset] == separator){
        skip_spaces_and_tabs(offset, line);
        return TRUE;
    }

    return FALSE;
}
