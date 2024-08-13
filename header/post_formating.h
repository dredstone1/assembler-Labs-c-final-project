#ifndef MMN14_POST_FORMATING_H
#define MMN14_POST_FORMATING_H

#include "file.h"

/**
 * @brief A structure to store a macro definition.
 */
typedef struct macro {
	line_text *lines; /* The lines of the macro */
	int number_of_macro_lines; /* The number of lines in the macro */
	int line_number; /* The line number of the macro definition */
	char macro_name[MAX_SYMBOL_SIZE]; /* The name of the macro */
} macro;


/**
 * @brief Processes and expands macros in an assembly source file.
 * 
 * This function reads the assembly source file(".as"). 
 * identifies macro definitions and calls, expands macro calls with their 
 * corresponding lines, and writes the pre-processed output to a 
 * new file with the ".am" extension.
 *
 * The function handles macro and reports any errors encountered 
 * during processing through the provided `error` structure.
 *
 * @param file_name The name of the assembly source file to process.
 * @param macros An array of `macro` structures to store identified macro definitions.
 * @param number_of_macros A pointer to the number of macros in the `macros` array.
 * @param error A pointer to an `error` structure to report any processing errors. 
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
