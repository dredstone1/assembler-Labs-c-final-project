#ifndef MMN14_UTILITIES_H
#define MMN14_UTILITIES_H

#include <stdlib.h>
#include "error.h"
#include <stdio.h>


typedef struct line_text{
	char content[MAX_LINE_LENGTH];
} line_text;


/**
 * @brief Converts an integer to its octal representation.
 *
 * @param num The integer to convert.
 * @return The octal representation of the integer.
 */
 int int_to_octal(int num);


/**
 * @brief Checks if there are extra characters in the given text.
 * 
 * @param line The text to check
 * @return 1 if there are extra characters, 0 otherwise
 */
int extra_char_at_end(char *line);


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
 * 
 * This function determines whether a line of text is a comment line, i.e. if it starts with a ;.
 * 
 * @param line The line of text to check.
 * @return 1 if the line is a comment line, 0 otherwise.
 */
int is_comment_line(char line[]);


/**
 * @brief Skips spaces and tabs in a line of text.
 * 
 * this function skips all spaces and tabs in a line of text, starting from a given offset,
 * and set the offset to the skipped offset.
 * 
 * @param offset A pointer to the offset in the line of text.
 * @param line The line of text to skip spaces and tabs in.
 */
void skip_spaces_and_tabs_with_offset(int *offset, char line[]);


/**
 * @brief Skips spaces and tabs in a line of text.
 * 
 * this function skips all spaces and tabs in a line of text, starting from a given offset,
 * set the offset to the skipped offset.
 * 
 * @param offset A pointer to the offset in the line of text.
 * @param line The line of text to skip spaces and tabs in.
 */
void skip_spaces_and_tabs(char **line);


/**
 * @brief Gets the next word in a line of text.
 * 
 * This function gets the next word in a line of text, starting from a given offset, 
 * and sets the offset to the end of the word.
 * 
 * @param workable_line A pointer to the line of text.
 * @param word A pointer to the word to store the next word in.
 * @param separators A string containing the separators to use.
 * @return The length of the next word.
 */
int get_next_word(char **workable_line, char word[], char separators[]);


/**
 * @brief Handles memory allocation.
 * 
 * This function allocates memory for a given size, and returns a pointer to the allocated memory.
 * if the allocation fails, it will print an error message.
 * 
 * @param size The size of the memory to allocate.
 * @param error The error data.
 * @return A pointer to the allocated memory, if successful, NULL otherwise.
 */
void *use_malloc(size_t size, error *error);


/**
 * @brief Handles memory reallocation.
 * 
 * This function reallocates memory for a given pointer and size, and returns a pointer to the reallocated memory.
 * if the pointer is NULL, it will allocate memory instead.
 * if the reallocation fails, it will print an error message.
 * 	
 * @param ptr A pointer to the memory to reallocate.
 * @param size The size of the memory to reallocate.
 * @param error The error data.
 * @return void* A pointer to the reallocated memory, if successful, NULL otherwise.
 */
void *use_realloc(void *ptr, size_t size, error *error);


/**
 * @brief Counts the number of commas until text is found.
 *
 * @param workable_line A pointer to the line of text.
 * @return The number of commas until text is found.
 */
int count_commas_until_text(char **workable_line);


/**
 * @brief Gets a line from a file.
 *
 * @param line The buffer to store the line.
 * @param line_number The line number to get.
 * @param file_name The name of the file to read from.
 * @param error The error array to store any errors in.
 * @return The length of the line read.
 */
int get_line_from_file(char line[], int line_number, char file_name[], error *error);


void go_to_next_line(FILE *file, char line[], int line_number, int *current_line_number);


/**
 * @brief check what type of opcode is the given string
 * 
 * @param str The string to check
 * @return opcode The opcode of the string
 */
opcode get_opcode_from_string(const char *str);


/**
 * @brief check what type of instruction is the given string
 * 
 * @param str The string to check
 * @return directive_type The directive of the string */
directive_type get_instruction_type_from_string(const char *str);


/**
 * @brief Check if a given string is a valid instruction
 * 
 * @param str The string to check
 * @return int 1 if its a valid instruction, 0 if its not a valid instruction
 */
int is_register(const char word[]);


/**
 * @brief Check if a given string is a valid directive
 * 
 * @param str The string to check
 * @return int 1 if its a valid directive, 0 if its not a valid directive
 */
int is_a_saved_word(const char word[]);


/**
 * @brief duplicate a string
 * 
 * @param source The string to duplicate
 * @param error The error array to store any errors in
 */
char* duplicate_string(const char *source, error *error);


/**
 * @brief search for the next white char in a string
 * 
 * @param str The string to check
 * @return char* A pointer to the next white char in the string, if found, NULL otherwise
 */
char* search_for_next_white_char(char *line);


/**
 * @brief 
 * 
 * @param str The string to check
 * @return char* A pointer to the next none white char in the string, if found, NULL otherwise
 */
int is_line_too_long(const char line[]);


/**
 * @brief replace the line ending with a null terminator
 * 
 * @param line The line to replace the ending for
 */
void replace_line_ending(char *line);
#endif