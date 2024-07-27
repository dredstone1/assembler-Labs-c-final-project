#ifndef MMN14_UTILITIES_H
#define MMN14_UTILITIES_H

#include <stdlib.h>
#include "error.h"


typedef struct line_text{
	char *content;
	int allocated;
} line_text;

/*colors*/

int int_to_octal(int num);

void get_next_word_n_skip(char line_text[], int *offset, char line[], char separators[], int separators_amount);
int extra_char_at_end(const char line[], int loc);


/**
 * @brief Checks if the given line is an empty line.
 * 
 * This function determines whether a line of text consists solely of whitespace characters 
 * (spaces and tabs) and/or is completely empty.
 * 
 * @param line The line of text to check.
 * @return 1 if the line is empty or contains only whitespace, 0 otherwise.
 */
int is_empty_line(char line[]);


/**
 * @brief Checks if the given line is a comment line.
 * This function determines whether a line of text is a comment line, i.e. if it starts with a ;.
 * 
 * @param line The line of text to check.
 * @return 1 if the line is a comment line, 0 otherwise.
 */
int is_comment_line(char line[]);


/**
 * @brief Skips spaces and tabs in a line of text.
 * this function skips all spaces and tabs in a line of text, starting from a given offset, set the offset to the skipped offset.
 * 
 * @param offset A pointer to the offset in the line of text.
 * @param line The line of text to skip spaces and tabs in.
 */
void skip_spaces_and_tabs_with_offset(int *offset, char line[]);

/**
 * @brief Skips spaces and tabs in a line of text.
 * this function skips all spaces and tabs in a line of text, starting from a given offset, set the offset to the skipped offset.
 * 
 * @param offset A pointer to the offset in the line of text.
 * @param line The line of text to skip spaces and tabs in.
 */
void skip_spaces_and_tabs(char **line);


/**
 * @brief gets the next word in a line of text.
 * This function gets the next word in a line of text, starting from a given offset, and sets the offset to the end of the word.
 * 
 * @param workable_line A pointer to the line of text.
 * @param word A pointer to the word to store the next word in.
 * @param separators A string containing the separators to use.
 */
int get_next_word(char **workable_line, char word[], char separators[]);


/**
 * @brief handle memory allocation.
 * This function allocates memory for a given size, and returns a pointer to the allocated memory.
 * 
 * @param size The size of the memory to allocate.
 * @param error The error array to store any errors in.
 * @return void* A pointer to the allocated memory.
 */
void *use_malloc(size_t size, error_array *error);


/**
 * @brief handle memory reallocation.
 * This function reallocates memory for a given pointer and size, and returns a pointer to the reallocated memory.
 * 	
 * @param ptr A pointer to the memory to reallocate.
 * @param size The size of the memory to reallocate.
 * @param error The error array to store any errors in.
 * @return void* A pointer to the reallocated memory.
 */
void *use_realloc(void *ptr, size_t size, error_array *error);


/**
 */
int count_commas_until_text(char **workable_line);


/**
 * @brief Check if a line is a macro definition line
 * 
 * @param line The line to check
 * @return int 1 if the line is a macro line, 0 if its not
 */
void free_lines(line_text *lines, int number_of_lines);


/**
 * @brief Check if a line is a macro definition line
 * 
 * @param line The line to check
 * @return int 1 if the line is a macro line, 0 if its not
 */
extern const char *opcode_names[16][3][1]; // Declare without initialization

#endif