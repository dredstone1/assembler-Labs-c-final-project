#include "../header/utilities.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>


/**
 * @brief Checks if a character is a separator.
 *
 * This function checks if a given character is present in the provided string of separators.
 *
 * @param c The character to check.
 * @param separators A string containing characters considered as separators.
 *
 * @return 1 if the character is a separator, 0 otherwise.
 */
int is_separator(char c, char separators[]);


int int_to_octal(int decimal_number) {
	/* Initialize the octal result and the place value */
	int octal_result = 0, place_value = 1;

	/* Iterate until the decimal number becomes zero */
	while (decimal_number != 0) {
		/* Calculate the remainder when dividing by 8 (the octal base) */
		/* Multiply the remainder by the current place value and add it to the octal result */
		octal_result += (decimal_number % BASE) * place_value;

		/* Divide the decimal number by 8 to move to the next place value */
		decimal_number /= BASE;

		/* Multiply the place value by 8 to move to the next position */
		place_value *= PLACE_VALUE_MULTIPLIER;
	}

	/* Return the calculated octal result */
	return octal_result;
}


int count_commas_until_text(char **workable_line) {
	/* Initialize comma counter */
	int count = 0;

	/* Handle NULL string input and return 0 if the string is NULL */
	if (*workable_line == NULL) {
		return 0;
	}

	/* Iterate through the string until non-whitespace and non-comma is encountered */
	while (**workable_line == ' ' || **workable_line == '\t' || **workable_line == ',') {
		/* Increment the counter for each comma */
		if (**workable_line == ',') {
			count++;
		}

		/* Move the pointer to the next character */
		(*workable_line)++;
	}

	/* Return the total count of commas */
	return count;
}


int get_next_word(char **workable_line, char word[], char separators[]) {
	/* Initialize word index */
	int i = 0;

	/* Handle NULL string input */
	if (*workable_line == NULL) {
		return 0;
	}

	/* Copy characters to word until a separator or end of string is reached */
	while (is_separator(**workable_line, separators) == 0 && **workable_line != '\0') {
		/* Copy current character to word buffer */
		word[i++] = **workable_line;

		/* Move to the next character in the input string */
		(*workable_line)++;
	}

	/* Null-terminate the extracted word */
	word[i] = '\0';

	/* Return the length of the extracted word */
	return i;
}


int is_separator(char c, char *separators) {
	/* Iterate through the separators string */
	while (*separators != '\0' && *separators != c) {
		separators++;
	}

	/* Return 1 if the character is a separator, 0 otherwise */
	return c == *separators;
}


int is_empty_line(char *line) {
	/* Skip leading spaces and tabs */
	skip_spaces_and_tabs(&line);

	/* Return 1 if the line is empty or contains only newline or carriage or is NULL
	 * Return 0 otherwise */
	return line == NULL || *line == '\0' || *line == '\n' || *line == '\r';
}


int is_comment_line(char *line) {
	/* Skip leading spaces and tabs */
	skip_spaces_and_tabs(&line);

	/* Return 1 if the line is a comment line (starts with a semicolon)
	 * Return 0 otherwise */
	return *line == COMMENT_CHAR;
}


void skip_spaces_and_tabs(char **line) {
	/* Skip leading spaces and tabs */
	while (*line != NULL && isspace(**line)) {
		(*line)++;
	}
}


void *use_malloc(size_t size, error *error) {
	/* attempt to allocate memory to the given size */
	void *ptr = (void *) malloc(size);

	/* if the allocation failed, print an error message, and set the error importance to CRITICAL */
	if (ptr == NULL) {
		print_system_error(MEMORY_ALLOCATION_FAILED_MESSAGE, error, CRITICAL);
	}

	/* return the allocated memory, or NULL if the allocation failed */
	return ptr;
}


void *use_realloc(void *ptr, size_t size, error *error) {
	/* check if the pointer is NULL, if it is,
	 * allocate memory using the use_malloc function */
	if (ptr == NULL) {
		ptr = use_malloc(size, error);
	}
		/* if the pointer is not NULL, it assumes that the memory was already allocated,
		 * so reallocate the memory using the realloc function */
	else {
		ptr = realloc(ptr, size);

		/* if the reallocation failed, print an error message, and set the error importance to CRITICAL */
		if (ptr == NULL) {
			print_system_error(MEMORY_ALLOCATION_FAILED_MESSAGE, error, CRITICAL);
		}
	}

	/* return the allocated memory, or NULL if the allocation\reallocation failed */
	return ptr;
}


int is_register(char *word) {
	/* check if the first character of the word is '*', if it is, increment the pointer by one */
	word += *word == '*';

	/* return 1 if all the following conditions are met:
	 *  - the first character is 'r',
	 *  - the second character is a digit between 0 and 9.
	 *  - the third character is '\0'.
	 * else return 0 */
	return *word == 'r' && word[1] >= '0' && word[1] <= '9' && word[2] == '\0';
}


int get_line_from_file(char line[], int line_number, char file_name[], error *error) {
	/* open the file in read mode */
	FILE *file = fopen(file_name, "r");
	/* if the file could not be opened, print an error message, and return 0 */
	if (file == NULL) {
		print_system_error(FILE_NOT_FOUND_MESSAGE, error, CANCELLATION);
		return 0;
	}

	/* read the file line by line until the desired line is reached */
	while (line_number > 0 && fgets(line, MAX_LINE_LENGTH, file) != NULL) {
		line_number--;
	}

	/* remove the newline or carriage from the end of the line, if it exists */
	remove_end_of_line(line);

	/* close the file */
	fclose(file);

	return 1;
}


void go_to_next_line(FILE *file, char line[], int line_number, int *current_line_number) {
	/* read the file line by line until the desired line is reached or the end of the file is reached */
	while (fgets(line, MAX_LINE_LENGTH, file) != NULL && *current_line_number < line_number) {
		/* increment the current line number */
		(*current_line_number)++;
	}
	
	remove_end_of_line(line);
}


void remove_end_of_line(char line[]) {
	/* initialize the length of the line */
	int length;

	/* check if the line is NULL, if it is, return */
	if (line == NULL) {
		return;
	}

	/* get the length of the line */
	length = strlen(line) - 1;

	/* remove the newline or carriage from the end of the line by replacing it with a null terminator,
	 * if it exists */
	if (line[length] == '\n' || line[length] == '\r') {
		line[length] = '\0';
	}
}


saved_word_type is_a_saved_word(char word[]) {
	/* check if the word is an opcode, and return the appropriate type */
	if (get_opcode_from_string(word) != -1) {
		return SAVED_WORD_OPCODE;
	}

	/* check if the word is a directive, and return the appropriate type */
	if (get_directive_type_from_string(word) != -1) {
		return SAVED_WORD_DIRECTIVE;
	}

	/* check if the word is a register, and return the appropriate type */
	if (is_register(word) == 1) {
		return SAVED_WORD_REGISTER;
	}

	/* check if the word is a macro command, and return the appropriate type */
	if (strcmp(word, MACRO) == 0 || strcmp(word, END_MACRO) == 0) {
		return SAVED_WORD_MACRO;
	}

	/* return NON_SAVED_WORD if the word is not a saved word */
	return NON_SAVED_WORD;
}


directive_type get_directive_type_from_string(char *str) {
	/* initialize the directive type */
	directive_type directive;

	/*increment the pointer by one if the first character is a dot */
	str += *str == '.';

	/* loop through the directive type names and check if the string matches any of them,
	 * if it does, return the current directive type */
	for (directive = 0; directive <= AMOUNT_DIRECTIVES_TYPES; directive++) {
		if (strcmp(str, directive_type_names[directive]) == 0) {
			return directive;
		}
	}
	
	/* the string does not match any of the directive type names, return -1 */
	return -1;
}


opcode get_opcode_from_string(const char *str) {
	/* initialize the opcode type */
	opcode code;

	/* loop through the opcode names and check if the string matches any of them,
	 * if it does, return the current opcode */
	for (code = 0; code <= AMOUNT_OPCODES; code++) {
		if (strcmp(str, opcode_names[code][0]) == 0) {
			return code;
		}
	}
	
	/* the string does not match any of the opcode names, return -1 */
	return -1;
}


char *duplicate_string(const char source[], error *error) {
	/* allocate memory for the new string */
	char *destination = (char *) use_malloc((strlen(source) + 1) * sizeof(char), error);
	
	/* if the allocation failed, return NULL */
	if (destination == NULL) {
		return NULL;
	}

	/* copy the source string to the destination string */
	strcpy(destination, source);
	
	/* return the destination string */
	return destination;
}


int is_line_too_long(const char line[]) {
	int i = 0;

	/* iterate through the line until the end of the line or the maximum line length is reached */
	while (line[i] != '\0' && line[i] != '\n' && line[i] != '\r' && i < MAX_LINE_LENGTH) {
		i++;
	}

	/* return 1 if the maximum line length is reached, 0 otherwise */
	return i < (MAX_LINE_LENGTH - 2);
}


void handle_free(void *ptr) {
	/* free the memory if the pointer is not NULL */
	if (ptr != NULL) {
		free(ptr);
	}
}


char *str_last_char(char *str, const char c) {
	/* initialize the last character pointer */
	char *last = NULL;
	
	/* check if the string is NULL or empty, if it is, return NULL */
	if (str == NULL || *str == '\0') {
		return NULL;
	}
	
	/* iterate through the string until the end of the string */
	while (*str != '\0') {
		/* if the current character is the desired character, set the last character pointer to the current character */
		last = *str == c ? str : last;
		
		/* move to the next character */
		str++;
	}

	/* return the last character pointer */
	return last;
}


int max(const int a, const int b) {
	/* return the maximum of the two numbers */
	return a > b ? a : b;
}