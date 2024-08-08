#ifndef MMN14_MACRO_H
#define MMN14_MACRO_H

#include <stdlib.h>
#include "error.h"
#include "utilities.h"
#include "post_formating.h"


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
int add_macro(char macro_name[], macro **macros, int *number_of_macros, error *error, int line_number);



/**
 * @brief Check if a line is an end macro line
 * 
 * @param line The line to check
 * @return int 1 if the line is an end macro line, 0 if its not
 */
int is_ending_macro(const char line[]);

/**
 * @brief Search for a macro by its name
 * 
 * @param macro_name The name of the macro to search for
 * @param macros The array of macros to search in
 * @param number_of_macros The number of macros in the array
 * @return int The index of the macro in the array, -1 if the macro was not found
 */
int search_macro_by_name(const char macro_name[], macro *macros, int number_of_macros);


/**
 * @brief Free the memory allocated for the macros
 * 
 * @param macros The macros array to free
 * @param number_of_macros The number of macros in the array
 */
void free_macros_lines(macro *macros, int number_of_macros);

#endif
