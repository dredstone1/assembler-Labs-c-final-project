#include "line.h"
#include "boolean.h"

bool is_char_separator(char c, char separators[], int separators_amount);

void skip_spaces_and_tags(int *offset, char line[]){

    while (line[*offset] == ' ' || line[*offset] == '\t')
        (*offset)++;
}

void get_next_word(char line_text[], int *offset, char line[], char separators[], int separators_amount, bool set_offset){
    int i = 0;

    while (is_char_separator(line[i+*offset], separators, separators_amount)==FALSE) {
        line_text[i] = line[i+*offset];
        i++;
    }

    line_text[i] = '\0';
    if (set_offset == TRUE)
        *offset += i;
}

void get_next_word_n_skip(char line_text[], int *offset, char line[], char separators[], int separators_amount, bool set_offset){
    skip_spaces_and_tags(offset, line);
    get_next_word(line_text, offset, line, separators, separators_amount, set_offset);
    skip_spaces_and_tags(offset, line);
}

bool is_char_separator(char c, char separators[], int separators_amount) {
    int i;
    for (i = 0; i < separators_amount; i++) {
        if (c == separators[i])
            return TRUE;
    }

    return FALSE;
}
