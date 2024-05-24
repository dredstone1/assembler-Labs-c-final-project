#include "line_data.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

line_command* line_command_set(int offset, char line[], char first_word[]);

void line_data_set(line_data *data, int offset, char line[]){
    char word[LINE_SIZE];
    get_next_word_n_skip(word, &offset, line, " \t\0", 3,FALSE);
/*
    printf("word: %d <-> %s\n", word[0], line);
*/
    if (word[0] == '.') {

    }
    else{
        data->command = line_command_set(offset, line, word);

    }
}

line_command* line_command_set(int offset, char line[], char first_word[LINE_SIZE]) {
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



/*void get_line_type(line_node *node){
    int i;

    node->line_data->directive = FALSE;

    for (i = node->line_data->offset; i < MAX_TAG_SIZE; i++) {
        if (node->line_text.content[i] == ' ' || node->line_text.content[i] == '\t')
            continue;
        else if (node->line_text.content[i] =='.') {
            node->line_data->directive = TRUE;
            node->line_data->offset = i+1;
        }

        return;
    }
}*/

