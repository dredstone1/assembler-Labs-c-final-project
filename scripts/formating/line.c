#include "line.h"

bool is_char_separator(char c, char separators[], int separators_amount);

void skip_spaces_and_tags(int *offset, char line[]){
    while (line[*offset] == ' ' || line[*offset] == '\t')
        (*offset)++;
}

void get_next_word(line_text *line_text, int *offset, char line[], char separators[], int separators_amount){
    int i = 0;

    while (is_char_separator(line[i+*offset], separators, separators_amount)==FALSE) {
        line_text->content[i] = line[i+*offset];
        i++;
    }

    line_text->content[i] = '\0';
    *offset += i;
}

void get_next_word_n_skip(line_text *line_text, int *offset, char line[], char separators[], int separators_amount){
    skip_spaces_and_tags(offset, line);
    get_next_word(line_text, offset, line, separators, separators_amount);
    skip_spaces_and_tags(offset, line);
}


bool is_char_separator(char c, char separators[], int separators_amount){
    int i;
    for(i=0; i<separators_amount; i++){
        if(c==separators[i])
            return TRUE;
    }

    return FALSE;
}

void set_direct_line_type(line_node *node){
    line_text word;

    get_next_word(&word, &node->line_data->offset, node->line_text.content, " \t,", 2);


/*    if(is_directive_type_is(node->line_text.content, &node->line_data->offset, DATA_DIRECTIVE))
        node->line_data->directive_line.type = DATA;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, STRING_DIRECTIVE))
        node->line_data->directive_line.type = STRING;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, ENTRY_DIRECTIVE))
        node->line_data->directive_line.type = ENTRY;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, EXTERN_DIRECTIVE))
        node->line_data->directive_line.type = EXTERN;
    else{

        *//*error undefined direct line type*//*
    }*/
}

void set_operation_line_type(line_node *node){
    skip_spaces_and_tags(&node->line_data->offset, node->line_text.content);

    if(is_directive_type_is(node->line_text.content, &node->line_data->offset, DATA_OPERATION))
        node->line_data->operation_line.type = MOV;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, STRING_OPERATION))
        node->line_data->operation_line.type = CMP;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, ENTRY_OPERATION))
        node->line_data->operation_line.type = ADD;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, EXTERN_OPERATION))
        node->line_data->operation_line.type = SUB;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, LEA_OPERATION))
        node->line_data->operation_line.type = LEA;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, CLR_OPERATION))
        node->line_data->operation_line.type = CLR;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, NOT_OPERATION))
        node->line_data->operation_line.type = NOT;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, INC_OPERATION))
        node->line_data->operation_line.type = INC;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, DEC_OPERATION))
        node->line_data->operation_line.type = DEC;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, JMP_OPERATION))
        node->line_data->operation_line.type = JMP;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, BNE_OPERATION))
        node->line_data->operation_line.type = BNE;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, RED_OPERATION))
        node->line_data->operation_line.type = RED;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, PRN_OPERATION))
        node->line_data->operation_line.type = PRN;
    else if(is_directive_type_is(node->line_text.content, &node->line_data->offset, JSR_OPERATION))
        node->line_data->operation_line.type = JSR;
    else if (is_directive_type_is(node->line_text.content, &node->line_data->offset, RTS_OPERATION))
        node->line_data->operation_line.type = RTS;
    else if (is_directive_type_is(node->line_text.content, &node->line_data->offset, STOP_OPERATION))
        node->line_data->operation_line.type = STOP;
    else{

        /*error undefined operation type*/
    }
}


