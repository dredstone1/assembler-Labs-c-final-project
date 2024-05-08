#ifndef MMN14_MACRO_H
#define MMN14_MACRO_H
#include "../systems/file.h"

#define MACRO "macr "

typedef struct macro_line{
    char name[LINE_SIZE];
} macro;

typedef struct macros{
    macro *macro_lines;
    int number_of_macro_lines;
    char macro_name[LINE_SIZE];
} macros;

bool is_line_macro(char line[]);

#endif //MMN14_MACRO_H
