#include "line_data.h"
#include <stdlib.h>

line_command* line_command_set(int offset, char line[]);

void line_data_set(line_data *data, int offset, char line[]){
    char word[LINE_SIZE];
    get_next_word_n_skip(word, &offset, line, " \t\0", 2);
    if (word[0] == '.') {
        data->command = line_command_set(offset, line);
    }
    else{

    }
}

line_command* line_command_set(int offset, char line[]){
    line_command *command = (line_command*)malloc(sizeof(line_command));
    char word[LINE_SIZE];
    get_next_word_n_skip(word, &offset, line, " \t\0", 2);

    command->opcode = get_opcode_from_string(word);
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

