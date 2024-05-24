#ifndef MMN14_LINE_H
#define MMN14_LINE_H

#include "line_node.h"
#include "boolean.h"

void get_next_word(char line_text[], int *offset, char line[], char separators[], int separators_amount, bool set_offset);
void skip_spaces_and_tags(int *offset, char line[]);

void get_next_word_n_skip(char line_text[], int *offset, char line[], char separators[], int separators_amount, bool set_offset);

#endif //MMN14_LINE_H
