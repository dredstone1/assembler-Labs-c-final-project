#include "line_data.h"
#include "directive.h"
#include <stdlib.h>
#include <string.h>

line_command* line_command_set(int offset, char line[], char first_word[]);
line_directive* line_directive_set(int offset, char line[], char first_word[]);

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



    return command;
}
