#ifndef MMN14_FIRST_PASS_H
#define MMN14_FIRST_PASS_H

#include "file.h"
#include "post_formating.h"


/**
 * @brief start the first pass of the assembler, and if it is possible to continue to the second pass	
 * 
 * This function reads the ".am" file line by line, and creates the word lists and symbol tables, to be used in the second pass.
 * at first, it opens the .am file, and then it reads the file line by line,
 * for each line:
 * - it checks if it is an empty line or a comment line, and if it is, it skips it.
 * - it remove the blank ending('\n', '\r') from the end of the line, and sets the workable line to the start workable line and copies the line to the workable line.
 * - it reads the first word of the line.
 * - it checks if the first word is a valid symbol, if it is, it adds it to the symbol table.
 * - if it is a symbol(end with ':') but not a valid symbol it will print the appropriate error, and continue to the next line.
 * - if it is a valid symbol but the same as a macro name or a saved word, it will print the appropriate error, and continue to the next line.
 * - if the first non symbol word in the line is a directive(it starts with '.'):
 * 		- it will call the handle_directive_type_line function, if there is any error, it will continue to the next line.
 * - else, the line is a command line:
 * 		- it will call the handle_command_type_line function, if there is any error, it will continue to the next line.
 * then it will check if there is any characters left in the line, if there is, it will print the appropriate error, and continue to the next line.
 * then if there isn't any critical or cancellation error and the file is not over, it will continue to the next line.
 * 
 * when it finishes reading the file, it will close the file.
 * 
 * if there isn't any critical or cancellation error:
 * - it will add to every symbol address in the symbol table IC + OS_SAVED_MEMORY_CELLS.
 * - it will call to the second pass function, to check if there are any symbols duplication or undefined symbols, and to set every symbol in the words list to the correct address.
 * 
 * if there wasn't any error/warning by now it will print the ".ob", ".ent" and ".ext" files.
 * 
 * at the end, it will free the allocated memory for the symbol table and entries table and word lists.
 *
 * @param file_name The name of the file to process.
 * @param error a pointer to the error structure.
 * @param macros a pointer to the macros array.
 * @param number_of_macros the number of macros in the macros array.
 */
void first_pass(char *file_name, error *error, macro *macros, int number_of_macros);

#endif
