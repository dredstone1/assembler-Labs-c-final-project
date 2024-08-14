#include "../header/error.h"
#include <stdio.h>
#include <string.h>


/**
 * @brief Prints the start of an error line with a specific format.
 *
 * This function prints a specified number of spaces followed by a formatted error line start.
 * It uses the `print_char_n_times` function to print the spaces.
 */
void print_start_line_error();


/**
 * @brief Starts the error message printing process.
 *
 * This function handles the common steps at the beginning of printing an error message.
 * It prints the file name and a separator line for the first error in a file,
 * For subsequent errors, it only prints a separator line. 
 * It then prints the error message provided as input.
 * it also increments the error count and sets the importance of the error to the given importance level.
 *
 * @param error A pointer to the error structure containing error details.
 * @param importance The importance level of the error.
 * @param error_massage The main error message to be printed.
 */
void start_printing_error_message_process(error *error, importance importance, char error_massage[]);


/**
 * @brief Prints a character a specified number of times.
 *
 * This function prints the given character `c` exactly `n` times to the standard output.
 *
 * @param c The character to be printed.
 * @param n The number of times to print the character.
 */
void print_char_n_times(char c, int n);


/**
 * @brief Prints the line number with a specific format for error messages.
 *
 * This function prints the given line number, formatted to be right-aligned within a 4-character width,
 * followed by a vertical bar (`|`). It is used to indicate the line number where an error occurred.
 *
 * @param line_number The line number to be printed.
 */
void print_start_line_number_error(int line_number);


/**
 * @brief Prints a tilde and caret marking for error highlighting.
 *
 * This function prints a series of tildes (`~`) and carets (`^`) to highlight specific portions of a line of code.
 * It is used to visually indicate the start and end positions of an error within the line.
 * it can also be used to mark the positions of two different errors in the same line.
 * 
 * if the given pos1 or pos2 is -1, it will not print a caret at that position.
 *
 * @param start The starting position of the error in the line.
 * @param end The ending position of the error in the line.
 * @param pos1 The first position for marking the caret.
 * @param pos2 The second position for marking the caret.
 */
void print_marker_and_caret(int start, int end, int pos1, int pos2, char line[]);


void print_system_error(char error_massage[], error *error, importance importance) {
	start_printing_error_message_process(error, importance, error_massage);
}


void print_general_error(char error_massage[], char error_description[], char line[], int line_number,
						 int start_place_in_line, int end_place_in_line, int mark_offset1, int mark_offset2,
						 error *error) {
	start_printing_error_message_process(error, WARNING, error_massage);
	
	/*print the line number and the line*/
	print_start_line_number_error(line_number);
	printf("%s\n", line);
	
	/*print the marker and the caret to indicate the error position*/
	print_start_line_error();
	print_marker_and_caret(start_place_in_line, end_place_in_line, mark_offset1, mark_offset2, line);

	printf("\n");
	
	/*print the error description with a quot of the error at the beginning of the description*/
	print_start_line_error();
	printf("%.*s%s%s\n", end_place_in_line - start_place_in_line, line + start_place_in_line,
		   ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE, error_description);
}


void print_start_line_error() {
	/*print the start vertical bar and the spaces at the beginning of the line*/
	print_char_n_times(' ', amount_of_spaces_at_start);
	printf("%s", start_VERTICAL_BAR_AT_START_OF_LINE);
}


void print_general_error_no_quoting(char error_massage[], char error_description[], char line[], int line_number,
									int start_place_in_line, int end_place_in_line, int mark_offset1, int mark_offset2,
									error *error) {
	start_printing_error_message_process(error, WARNING, error_massage);

	/*print the line number and the line*/
	print_start_line_number_error(line_number);
	printf("%s\n", line);

	/*print the marker and the caret to indicate the error position*/
	print_start_line_error();
	print_marker_and_caret(start_place_in_line, end_place_in_line, mark_offset1, mark_offset2, line);

	/*print the error description without a quot*/
	printf("\n");
	print_start_line_error();
	printf("%s\n", error_description);
}


void print_two_line_error(char error_massage[], char error_description[], char line1[], char line2[], int line1_number,
						  int line2_number, int start_place_in_line1, int end_place_in_line1, int start_place_in_line2,
						  int end_place_in_line2, error *error) {
	start_printing_error_message_process(error, WARNING, error_massage);

	/*print the line number and the line*/
	print_start_line_number_error(line1_number);
	printf("%s\n", line1);
	
	/*print the marker and the caret to indicate the error position*/
	print_start_line_error();
	print_marker_and_caret(start_place_in_line1, end_place_in_line1, -1, -1, line1);
	printf("\n");

	/*print the line number and the line*/
	print_start_line_number_error(line2_number);
	printf("%s\n", line2);

	/*print the marker and the caret to indicate the error position*/
	print_start_line_error();
	print_marker_and_caret(start_place_in_line2, end_place_in_line2, -1, -1, line2);

	
	/*print the error description with a quot of the error at the beginning of the description*/
	printf("\n");
	print_start_line_error();
	if (start_place_in_line1 == 0) {
		printf("%.*s%s%s", end_place_in_line1 - start_place_in_line1, &line1[start_place_in_line1],
			   ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE, error_description);
	} else {
		printf("%.*s%s%s", end_place_in_line1 - start_place_in_line1+1, &line1[start_place_in_line1],
			   ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE, error_description);
	}
	
	/*print the line number to indicate the line number of the first line*/
	printf(" %d\n", line1_number);
}


void print_macro_b_label_same(char line[], int line_number, int macro_line_number, error *error, char macro[]) {
	start_printing_error_message_process(error, WARNING, SYMBOL_CAN_BE_SAME_AS_MACRO_NAME);

	/*print the line number and the line*/
	print_start_line_number_error(line_number);
	printf("%s\n", line);

	/*print the marker and the caret to indicate the error position*/
	print_start_line_error();
	print_marker_and_caret(strstr(line, macro) - line, strstr(line, macro) - line + strlen(macro), -1, -1, line);

	/*print the error description with a quot of the name of the macro at the beginning of the description*/
	printf("\n");
	print_start_line_error();
	printf("%.*s%s%s\n", (int)strlen(macro), macro, ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE,
		   SYMBOL_CAN_BE_SAME_AS_MACRO_NAME_DESCRIPTION);
	
	/*print the line number to indicate the line number of the macro and the file name were the macro is defined*/
	print_start_line_error();
	set_ending_to_file_name(error->file_name, SOURCE_FILE_ENDING);
	printf(SYMBOL_ALREADY_EXISTS_AS_MACRO_DESCRIPTION, error->file_name, macro_line_number + 1);
	printf("\n");
	set_ending_to_file_name(error->file_name, SOURCE_FILE_AFTER_POST_ASSEMBLER_ENDING);
}


void print_command_not_legal(char variable[], char line[], int line_number, error *error,
							 const char variable_type_valid[], int usage, opcode code) {
	int legal_printed = 0, i;
	start_printing_error_message_process(error, WARNING, INVALID_VARIABLE_TYPE_MESSAGE);

	/*print the line number and the line*/
	print_start_line_number_error(line_number);
	printf("%s\n", line);

	/*print the marker and the caret to indicate the error position*/
	print_start_line_error();
	print_marker_and_caret(strstr(line, variable) - line, strstr(line, variable) - line + strlen(variable), -1,
						   -1, line);
	printf("\n");
	
	/*print the error description with a quot of the variable at the beginning of the description,
	 * depending on the usage, source or destination*/
	print_start_line_error();
	if (usage == 0) {
		printf("%s %s", variable, INVALID_VARIABLE_TYPE_SOURCE_DESCRIPTION);
	} else {
		printf("%s %s", variable, INVALID_VARIABLE_TYPE_DESTINATION_DESCRIPTION);
	}

	/*print the opcode name and the connection word between the opcode and the valid variable types*/
	printf("%s %s", opcode_names[code][0], LEGAL_VARIABLE_TYPES_MESSAGE);

	/*print the valid variable types,
	 * by iterating over the variable type valid array and printing the valid variable types names*/
	for (i = 0; i < 4; i++) {
		if (variable_type_valid[i] != '_') {
			if (legal_printed > 0) {
				printf(", ");
			}

			printf("%s", operand_names[i]);
			legal_printed++;
		}
	}

	printf("\n");
}


void print_macro_already_exists(const char macro[], char line[], int line_number, error *error, int line_number_exist) {
	start_printing_error_message_process(error, WARNING, ILLEGAL_MACRO_NAME_MESSAGE);

	/*print the line number and the line*/
	print_start_line_number_error(line_number);
	printf("%s\n", line);

	/*print the marker and the caret to indicate the error position*/
	print_start_line_error();
	print_marker_and_caret(strstr(line, macro) - line, strstr(line, macro) - line + strlen(macro), -1, -1, line);

	/*print the error description with a quot of the name of the macro at the beginning of the description*/
	printf("\n");
	print_start_line_error();
	printf("%.*s%s%s %d\n", (int)strlen(macro), macro, ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE,
		   MACRO_NAME_ALREADY_EXISTS_DESCRIPTION, line_number_exist);
}


void print_start_line_too_long_error(int line_number, error *error) {
	start_printing_error_message_process(error, WARNING, LINE_TOO_LONG_MESSAGE);
	/*print the line number*/
	print_start_line_number_error(line_number);
}


void print_end_line_too_long_error() {
	print_start_line_error();
	
	/*print the line too long description*/
	printf("%s\n", LINE_TOO_LONG_DESCRIPTION);
}


void print_char_n_times(char c, int n) {
	int i;
	/*iterate over the given number of times and print the given character*/
	for (i = 0; i < n; i++) {
		printf("%c", c);
	}
}


void print_start_line_number_error(int line_number) {
	/*print the line_number number in amount_of_spaces_at_start width, and then print the vertical bar*/
	printf("%*d%s", amount_of_spaces_at_start, line_number, start_VERTICAL_BAR_AT_START_OF_LINE);
}


void print_marker_and_caret(int start, int end, int pos1, int pos2, char line[]) {
	int i;

	/*iterate over the length of the line until it reaches the given end position*/
	for (i = 0; i < end; ++i) {
		/*if the current position is equal to the first or second position, print a caret*/
		if ((pos1 != -1 && i == pos1) || (pos2 != -1 && i == pos2)) {
			/*if the current character in the given line is a tab, print a space length of the tab length*/
			if (line[i] == '\t') {
				print_char_n_times(' ', TAB_LENGTH);
			}
			printf("^");
		}
			/* else if the current position is before the start position,print a space or a tab depending on the character in the line*/
		else if (i < start) {
			if (line[i] == '\t') {
				printf("\t");
			} else {
				printf(" ");
			}
		}
			/* else the current position is between the start and end positions, print a tilde */
		else {
			printf("~");
		}
	}
}


void print_separator(int from_beginning) {
	/*if the separator is not to be printed from the beginning of the line,
	 *print a separator line with the length of the line and the start vertical bar*/
	if (from_beginning == 0) {
		print_start_line_error();
		print_char_n_times('-', MAX_LINE_LENGTH);
	}
		/* else print a separator line with the length of the line + the length of the side vertical bars */
	else {
		print_char_n_times('-', MAX_LINE_LENGTH + SIDE_LENGTH);
	}

	printf("\n");
}


void set_ending_to_file_name(char *fileName, char ending[]) {
	/*copy the given ending to the file name after the last dot in the file name*/
	strcpy(strchr(fileName, '.') + 1, ending);
}

void start_printing_error_message_process(error *error, importance importance, char error_massage[]) {
	/*if the error count is 0, print the file name and from the beginning of the line*/
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}
		/*if the error count is greater than 0, print a separator line not from the beginning of the line, without the file name*/
	else if (error->error_single_file_count > 0) {
		print_separator(0);
	}
	
	/*print the start bars and the general error message*/
	print_start_line_error();
	printf("%s\n", error_massage);

	/*increment the error count and set the importance of the error*/
	error->error_single_file_count++;
	error->importance = importance;
}