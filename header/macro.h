#ifndef MMN14_MACRO_H
#define MMN14_MACRO_H

#include <stdlib.h>
#include "error.h"

#define MACRO "macr"
#define END_MACRO "endmacr"

typedef struct macro{
    line_node *first_macro_lines;
    line_node *last_macro_lines;
    int number_of_macro_lines;
    char macro_name[LINE_SIZE];
} macro;

/*typedef struct macros{
    macro *macro;
    int number_of_macros;
} macros;*/


/**
 * @brief Check if a line is a macro definition line
 * 
 * @param line The line to check
 * @return int 1 if the line is a macro line, 0 if its not
 */
int is_line_macro(const char line[]);


/**
 * @brief add a macro to a macro array
 * 
 * @param macro_name The name of the macro
 * @param macros The macros struct to add the macro to
 * @param error The error array to add errors to
*/
void add_macro(char macro_name[], macro **macros, int *number_of_macros, error_array *error);



line_node *read_macro_lines(line_node **head);
macro* get_macro_from_name(macro *macros, line_node *node);
void replace_line_to_macro(macro macro, line_node **node);
void free_macros(macro *macros);
void handle_macros(line_node **first_file_node,int *number_of_rows, macro *macros, error_array *error);

#endif
