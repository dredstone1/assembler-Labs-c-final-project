#ifndef MMN14_LINE_H
#define MMN14_LINE_H

#include "line_node.h"
#include "boolean.h"

void get_next_word(char line_text[], int *offset, char line[], char separators[], int separators_amount);
void skip_spaces_and_tabs(int *offset, char line[]);

void get_next_word_n_skip(char line_text[], int *offset, char line[], char separators[], int separators_amount);
int count_char_until_not_separator(char line[], char c, int *offset, char separators[], int separators_amount);
#endif //MMN14_LINE_H
