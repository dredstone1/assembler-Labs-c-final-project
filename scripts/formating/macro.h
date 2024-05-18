#ifndef MMN14_MACRO_H
#define MMN14_MACRO_H

#include <stdlib.h>
#include "line.h"
#include "../data/pos.h"
#include "../systems/error.h"

#define MACRO "macr"
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

bool is_line_macro(const char line[]);
void add_macro(char macro_name[], line_node **node, macros *macros);
void set_macro_name(const char line[], macro *macro, pos *pos);
line_node **read_macro_lines(line_node **head);
macro* get_macro_from_name(macros *macros, line_node *node);
void replace_line_to_macro(macro macro, line_node **node);

#endif //MMN14_MACRO_H
