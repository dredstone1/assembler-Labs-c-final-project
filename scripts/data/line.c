#include "line.h"
#include "boolean.h"

bool is_char_separator(char c, char separators[], int separators_amount);

void skip_spaces_and_tabs(int *offset, char line[]){
    while (line[*offset] == ' ' || line[*offset] == '\t')
        (*offset)++;
}

void get_next_word(char line_text[], int *offset, char line[], char separators[], int separators_amount){
    int i = 0;

    while (is_char_separator(line[i+*offset], separators, separators_amount)==FALSE) {
        line_text[i] = line[i+*offset];
        i++;
    }

    line_text[i] = '\0';
    *offset += i;
}

void get_next_word_n_skip(char line_text[], int *offset, char line[], char separators[], int separators_amount){
    skip_spaces_and_tabs(offset, line);
    get_next_word(line_text, offset, line, separators, separators_amount);
    skip_spaces_and_tabs(offset, line);
}

bool is_char_separator(char c, char separators[], int separators_amount) {
    int i;
    for (i = 0; i < separators_amount; i++) {
        if (c == separators[i])
            return TRUE;
    }

    return FALSE;
}
