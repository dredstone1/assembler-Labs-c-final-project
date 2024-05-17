#ifndef MMN14_LINE_H
#define MMN14_LINE_H

#include "../data/tag.h"
#include "directive_line_type.h"
#include "operation_line_type.h"
#include "line_node.h"


void get_next_word(line_text *line_text, int *offset, char line[], char separators[], int separators_amount);
void skip_spaces_and_tags(int *offset, char line[]);

void set_direct_line_type(line_node *node);
void set_operation_line_type(line_node *node);

#endif //MMN14_LINE_H
