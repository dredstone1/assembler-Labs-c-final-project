#ifndef MMN14_POST_FORMATING_H
#define MMN14_POST_FORMATING_H

#include "file.h"


typedef struct macro{
	line_text *lines;
	int number_of_macro_lines;
	int line_number;
	char macro_name[MAX_SYMBOL_SIZE];
} macro;


/**
 *
 */
void post_formating(error *error, char file_name[], macro **macros, int *number_of_macros);


/**
 * @brief Searches for a macro by its name.
 * 
 * This function iterates through the list of macros to find a macro with the specified name.
 * 
 * @param macro_name The name of the macro to search for.
 * @param macros An array of macros.
 * @param number_of_macros The number of macros in the array.
 * @return int The index of the macro if found, -1 otherwise.
 */
int search_macro_by_name(const char macro_name[], macro *macros, int number_of_macros);
#endif
