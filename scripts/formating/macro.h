#ifndef MMN14_MACRO_H
#define MMN14_MACRO_H

#include <stdlib.h>
#include "line.h"
#include "../data/pos.h"
#include "../systems/error.h"

#define MACRO "macr "
#define END_MACRO "endmacr"


typedef struct macro{
    line_node macro_lines;
    int number_of_macro_lines;
    char macro_name[LINE_SIZE];
} macro;

typedef struct macros{
    macro *macro;
    int number_of_macros;
} macros;

bool is_line_macro(const char line[], int *offset);
void set_macro_name(const char line[], macro *macro, pos *pos, error *error);

#endif //MMN14_MACRO_H
