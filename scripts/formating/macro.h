#ifndef MMN14_MACRO_H
#define MMN14_MACRO_H

#include <stdlib.h>
#include "line.h"
#include "../data/pos.h"
#include "../systems/error.h"

#define MACRO "macr "
#define END_MACRO "endmacr"


typedef struct macro{
    line_node *macro_lines;
    int number_of_macro_lines;
    char macro_name[LINE_SIZE];
} macro;

typedef struct macros{
    macro *macro;
    int number_of_macros;
} macros;

bool is_line_macro(const char line[], pos *pos);
void set_macro_name(const char line[], macro *macro, pos *pos);
void get_macro_lines(line_node **node, macro *macro, error *error);
macro* is_line_call_macro(macros *macros, line_node *node, error *error);
void push_to_macro(line_node **prev, line_node *new);
#endif //MMN14_MACRO_H
