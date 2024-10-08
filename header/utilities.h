#ifndef MMN14_UTILITIES_H
#define MMN14_UTILITIES_H

#include <stdlib.h>
#include "error.h"
#include <stdio.h>


#define BASE 8 /* the base of the octal number */
#define PLACE_VALUE_MULTIPLIER 10 /* the multiplier to move to the next place value */

#define COMMENT_CHAR ';' /* the character that represents a start of a comment */

#define AMOUNT_DIRECTIVES_TYPES 4 /* the amount of directive types */
#define AMOUNT_OPCODES 16 /* the amount of opcodes */

/**
 * @brief structure to store a line of text.
 */
typedef struct line_text {
	char content[MAX_LINE_LENGTH];
} line_text;


/**
 * @brief Enum representing the different types of saved words.
 */
typedef enum {
	SAVED_WORD_OPCODE, /* The word is an opcode */
	SAVED_WORD_DIRECTIVE, /* The word is a directive */
	SAVED_WORD_REGISTER, /* The word is a register */
	SAVED_WORD_MACRO, /* The word is a macro */
	NON_SAVED_WORD /* The word is not a saved word */
} saved_word_type;


/**
 * @brief Converts a decimal integer to its octal representation.
 *
 * This function takes a decimal integer as input and converts it to its octal
 * representation. The conversion is done by repeatedly dividing the decimal
 * number by 8 and accumulating the remainders in the correct place value.
 *
 * @param decimal_number The decimal integer to be converted to octal.
 * @return The octal representation of the input decimal integer.
 */
int int_to_octal(int num);


/**
 * @brief Checks if the given line is an empty line.
 * 
 * This function determines whether a line of text consists solely of whitespace characters (spaces and tabs) and/or is completely empty,
 * by skipping all spaces and tabs in the line,
 * and then to check the current character is the end of the line('\0' or '\n' or '\r').
 * 
 * @param line The line of text to check.
 * @return 1 if the line is empty or contains only whitespace, 0 otherwise.
 */
int is_empty_line(char *line);


/**
 * @brief Checks if a given line is a comment line.
 *
 * a comment line is a line that starts with a semicolon (';').
 * This function determines if the provided line of text is a comment line,
 * by skipping all spaces and tabs in the line,
 * and then checking if the first non-space/tab character is a semicolon (';').
 *
 * @param line The line of text to check.
 * @return 1 if the line is a comment line, 0 otherwise.
 */
int is_comment_line(char line[]);


/**
 * @brief Skips spaces and tabs in a given line by updating the line pointer.
 *
 * This function advances the line pointer to skip over any spaces (' ') or tabs ('\t')
 * in the provided line. It continues to increment the pointer until a non-space
 * and non-tab character is encountered.
 *
 * @param line Pointer to a character pointer representing the current position in the line.
 *             This pointer will be updated to point to the first non-space and non-tab character.
 */
void skip_spaces_and_tabs(char **line);


/**
 * @brief Extracts the next word from a line, using specified separators.
 *
 * This function reads characters from the provided line and stores them in the word array
 * until a separator or the end of the line is encountered. The line pointer is updated
 * to point to the character following the extracted word.
 *
 * @param workable_line Pointer to a character pointer representing the current position in the line.
 *                      This pointer will be updated to point to the character after the extracted word.
 * @param word          The character array where the extracted word will be stored.
 * @param separators    The character array containing separator characters.
 * @return The length of the extracted word.
 */
int get_next_word(char **workable_line, char word[], char separators[]);


/**
 * @brief Allocates memory and handles errors if allocation fails.
 *
 * This function attempts to allocate a block of memory of the specified size using `malloc`.
 * If the allocation fails, it prints a system error message and sets the error importance to CRITICAL.
 *
 * @param size The size of the memory block to be allocated.
 * @param error Pointer to an error structure that will be updated if the allocation fails.
 * @return A pointer to the allocated memory block, or NULL if the allocation fails.
 */
void *use_malloc(size_t size, error *error);


/**
 * @brief Reallocates memory and handles errors if reallocation fails.
 *
 * This function attempts to reallocate a block of memory to the specified size using `realloc`.
 * If the provided pointer is NULL, it allocates a new block of memory using `use_malloc`.
 * If the reallocation or allocation fails, it prints a system error message and sets the error importance to CRITICAL.
 *
 * @param ptr Pointer to the memory block to be reallocated. If NULL, a new block is allocated.
 * @param size The new size of the memory block.
 * @param error Pointer to an error structure that will be updated if the reallocation fails.
 * @return A pointer to the reallocated (or newly allocated) memory block, or NULL if the reallocation fails.
 */
void *use_realloc(void *ptr, size_t size, error *error);


/**
 * @brief Counts the number of commas in a line until a non-space, non-tab, and non-comma character is encountered.
 *
 * This function increments the line pointer to skip over spaces (' '), tabs ('\t'), and commas (',').
 * It counts the number of commas encountered until a character that is not a space, tab, or comma is found.
 *
 * @param workable_line Pointer to a character pointer representing the current position in the line.
 *                      This pointer will be updated to point to the first non-space, non-tab, and non-comma character.
 * @return The count of commas encountered.
 */
int count_commas_until_text(char **workable_line);


/**
 * @brief Retrieves a specific line from a file.
 *
 * This function opens a file and reads lines until the specified line number is reached.
 * If the file cannot be opened, it prints a system error message and sets the error importance to CRITICAL.
 * The retrieved line is stored in the provided line array, and any trailing newline or carriage return character is removed.
 *
 * @param line The character array where the retrieved line will be stored.
 * @param line_number The number of the line to retrieve.
 * @param file_name The name of the file to read from.
 * @param error Pointer to an error structure that will be updated if the file cannot be opened.
 *
 * @return 1 if the line is successfully retrieved, 0 if the file cannot be opened.
 */
int get_line_from_file(char line[], int line_number, char file_name[], error *error);


/**
 * @brief Advances the file pointer to a specified line number.
 *
 * This function reads lines from the file until the specified line number is reached.
 * It updates the current line number accordingly. If the last character of the line is a newline
 * or carriage return, it is removed.
 *
 * @param file Pointer to the file to read from.
 * @param line The character array where the current line will be stored.
 * @param line_number The target line number to advance to.
 * @param current_line_number Pointer to an integer representing the current line number. This value will be updated to the target line number.
 */
void go_to_next_line(FILE *file, char line[], int line_number, int *current_line_number);


/**
 * @brief Retrieves the opcode corresponding to a given string.
 *
 * This function iterates through the list of opcodes and compares each one with the provided string.
 * If a match is found, the corresponding opcode is returned. If no match is found, the function returns -1.
 *
 * @param str The string representing the opcode name to be matched.
 * @return The corresponding opcode if a match is found, otherwise -1.
 */
opcode get_opcode_from_string(const char *str);


/**
 * @brief Retrieves the directive type corresponding to a given string.
 * 
 * This function iterates through the list of directive types and compares each one with the provided string.
 * If a match is found, the corresponding directive type is returned. If no match is found, the function returns -1.
 *
 * This function is used to determine the type of a directive based on its name,
 * even if the starting period (.) is not included in the string.
 * 
 * @param str The string representing the directive name to be matched.
 * @return The corresponding directive type if a match is found, otherwise -1.
 */
directive_type get_directive_type_from_string(char *str);


/**
 * @brief Validates if a given string represents a register.
 * 
 * This function checks if the provided string is a valid register name.
 * A valid register name:
 *  - May start with an asterisk (*).
 *  - Must start with the letter 'r' (after the optional asterisk).
 *  - Must be followed by a single digit (0-9).
 *  - Must not contain any other characters.
 * 
 * @param word The string to validate.
 * @return int 1 if the string is a valid register, 0 otherwise.
 */
int is_register(char *word);


/**
 * @brief Determines the type of a saved word.
 * 
 * This function checks if the provided string is a saved word and returns its type.
 * A saved word can be:
 * - An opcode
 * - A directive
 * - A register
 * - A macro command, such as "macr" and "endmacr"
 * 
 * @param word The string to check.
 * @return saved_word_type The type of the saved word, or NON_SAVED_WORD if it is not a saved word.
 */
saved_word_type is_a_saved_word(char word[]);


/**
 * @brief Duplicates a given string.
 * 
 * This function allocates memory and copies the provided source string to the new memory block.
 * If the memory allocation fails, the function returns NULL.
 * 
 * @param source The string to duplicate.
 * @param error A pointer to an error structure to handle memory allocation errors.
 * @return char* A pointer to the duplicated string if successful, NULL otherwise.
 */
char *duplicate_string(const char source[], error *error);


/**
 * @brief Checks if a line exceeds the maximum allowed length.
 * 
 * This function iterates through the given line to determine if it exceeds the defined maximum length.
 * If the length to the first ending character ('\0', '\n', or '\r') is greater than the maximum allowed length,
 * the function returns 0. Otherwise, it returns 1.
 * 
 * @param line The string to check.
 * @return int 1 if the line is within the allowed length, 0 if it exceeds the limit.
 */
int is_line_too_long(const char line[]);


/**
 * @brief Frees the memory pointed to by ptr if it is not NULL.
 *
 * This function checks if the provided pointer is not NULL.
 * If the pointer is not NULL, it frees the memory block it points to.
 *
 * @param ptr A pointer to the memory block to be freed.
 */
void handle_free(void *ptr);


/**
 * @brief Finds the last occurrence of a character in a string.
 *
 * This function iterates through a given string and returns a pointer
 * to the last occurrence of the specified character. If the character
 * is not found or the input string is NULL or empty, it returns NULL.
 *
 * @param str A pointer to the input string.
 * @param c The character to search for.
 * @return A pointer to the last occurrence of 'c' in 'str', or NULL if 'c' is not found or 'str' is NULL or
 * has no characters.
 */
char* str_last_char(char* str, const char c);


/**
 * @brief Removes the end of line characters from a string.
 *
 * This function removes the newline ('\n') or carriage return ('\r') characters, whatever comes first,
 * from the end of the given string. If the string is NULL or empty, it does nothing.
 *
 * @param line The string to remove the end of line characters from.
 */
void remove_end_of_line(char line[]);


/**
 * @brief Compares two integers and returns the bigger number.
 * 
 * This function compares two integers and returns the bigger of the two.
 *
 * @param a The first integer to compare.
 * @param b The second integer to compare.
 * @return int The bigger of the two integers.
 */
int max(const int a, const int b);

#endif