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
 * @param start The starting position of the error in the line.
 * @param end The ending position of the error in the line.
 * @param pos1 The first position for marking the caret.
 * @param pos2 The second position for marking the caret.
 */
void print_tilde_and_caret(int start, int end, int pos1, int pos2, char line[]);


void print_system_error(char error_massage[]) {
	printf("%s\n", error_massage);
}


void print_general_error(char error_massage[], char error_description[], char line[], int line_number,
						 int start_place_in_line, int end_place_in_line, int mark_offset1, int mark_offset2,
						 error *error) {
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}
	if (error->error_single_file_count > 0) {
		print_separator(0);
	}

	print_start_line_error();
	printf("%s\n", error_massage);

	print_start_line_number_error(line_number);
	printf("%s\n", line);
	print_start_line_error();
	print_tilde_and_caret(start_place_in_line, end_place_in_line, mark_offset1, mark_offset2, line);

	printf("\n");
	print_start_line_error();
	printf("%.*s%s%s\n", end_place_in_line - start_place_in_line, line + start_place_in_line,
		   ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE, error_description);

	error->error_single_file_count++;
	error->error_count++;
}


void print_start_line_error() {
	print_char_n_times(' ', amount_of_spaces_at_start);
	printf("%s", start_format_line_error);
}


void print_general_error_no_quoting(char error_massage[], char error_description[], char line[], int line_number,
									int start_place_in_line, int end_place_in_line, int mark_offset1, int mark_offset2,
									error *error) {
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}
	if (error->error_single_file_count > 0) {
		print_separator(0);
	}

	print_start_line_error();
	printf("%s\n", error_massage);

	print_start_line_number_error(line_number);
	printf("%s\n", line);
	print_start_line_error();
	print_tilde_and_caret(start_place_in_line, end_place_in_line, mark_offset1, mark_offset2, line);

	printf("\n");
	print_start_line_error();

	printf("%s\n", error_description);

	error->error_single_file_count++;
	error->error_count++;
}


void print_two_line_error(char error_massage[], char error_description[], char line1[], char line2[], int line1_number,
						  int line2_number, int start_place_in_line1, int end_place_in_line1, int start_place_in_line2,
						  int end_place_in_line2, error *error) {
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}
	if (error->error_single_file_count > 0) {
		print_separator(0);
	}

	print_start_line_error();
	printf("%s\n", error_massage);

	print_start_line_number_error(line1_number);
	printf("%s\n", line1);
	print_start_line_error();
	print_tilde_and_caret(start_place_in_line1, end_place_in_line1, -1, -1, line1);
	printf("\n");

	print_start_line_number_error(line2_number);
	printf("%s\n", line2);
	print_start_line_error();
	print_tilde_and_caret(start_place_in_line2, end_place_in_line2, -1, -1, line2);

	printf("\n");
	print_start_line_error();

	if (start_place_in_line1 == 0) {
		printf("%.*s%s%s", end_place_in_line1 - start_place_in_line1, &line1[start_place_in_line1],
			   ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE, error_description);
	} else {
		printf("%.*s%s%s", end_place_in_line1 - start_place_in_line1, &line1[start_place_in_line1 - 1],
			   ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE, error_description);
	}
	printf(" %d\n", line1_number);

	error->error_single_file_count++;
	error->error_count++;
}


void print_macro_b_label_same(char line[], int line_number, int macro_line_number, error *error, char macro[]) {
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}
	if (error->error_single_file_count > 0) {
		print_separator(0);
	}

	print_start_line_error();
	printf("%s\n", SYMBOL_CAN_BE_SAME_AS_MACRO_NAME);

	print_start_line_number_error(line_number);
	printf("%s\n", line);

	print_start_line_error();
	print_tilde_and_caret(strstr(line, macro) - line, strstr(line, macro) - line + strlen(macro), -1, -1, line);

	printf("\n");
	print_start_line_error();
	printf("%.*s%s%s\n", (int) strlen(macro), macro, ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE,
		   SYMBOL_CAN_BE_SAME_AS_MACRO_NAME_DESCRIPTION);

	print_start_line_error();
	set_ending_to_file_name(error->file_name, SOURCE_FILE_ENDING);
	printf(SYMBOL_ALREADY_EXISTS_AS_MACRO_DESCRIPTION, error->file_name, macro_line_number + 1);
	printf("\n");
	set_ending_to_file_name(error->file_name, SOURCE_FILE_AFTER_POST_ASSEMBLER_ENDING);

	error->error_single_file_count++;
	error->error_count++;
}


void print_command_not_legal(char variable[], char line[], int line_number, error *error,
							 const char variable_type_valid[], int usage, opcode code) {
	int legal_printed = 0, i;

	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}
	if (error->error_single_file_count > 0) {
		print_separator(0);
	}

	print_start_line_error();
	printf("%s\n", INVALID_VARIABLE_TYPE_MESSAGE);

	print_start_line_number_error(line_number);
	printf("%s\n", line);

	print_start_line_error();
	print_tilde_and_caret(strstr(line, variable) - line, strstr(line, variable) - line + strlen(variable), -1,
						  -1, line);
	printf("\n");

	print_start_line_error();
	if (usage == 0) {
		printf("%s %s", variable, INVALID_VARIABLE_TYPE_SOURCE_DESCRIPTION);
	} else {
		printf("%s %s", variable, INVALID_VARIABLE_TYPE_DESTINATION_DESCRIPTION);
	}

	printf("%s %s", opcode_names[code][0], LEGAL_VARIABLE_TYPES_MESSAGE);

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

	error->error_single_file_count++;
	error->error_count++;
}


void print_start_line_too_long_error(int line_number, error *error) {
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}
	if (error->error_single_file_count > 0) {
		print_separator(0);
	}

	print_start_line_error();
	printf("%s\n", LINE_TOO_LONG_MESSAGE);

	print_start_line_number_error(line_number);
}


void print_end_line_too_long_error() {
	print_start_line_error();
	printf("%s\n", LINE_TOO_LONG_DESCRIPTION);
}


void print_char_n_times(char c, int n) {
	int i;
	for (i = 0; i < n; i++) {
		printf("%c", c);
	}
}


void print_start_line_number_error(int line_number) {
	printf("%4d | ", line_number);
}


void print_tilde_and_caret(int start, int end, int pos1, int pos2, char line[]) {
	int i;

	for (i = 0; i < end; ++i) {
		if ((pos1 != -1 && i == pos1) || (pos2 != -1 && i == pos2)) {
			if (line[i] == '\t') {
				print_char_n_times(' ', 4);
			}
			printf("^");
		} else if (i < start) {
			if (line[i] == '\t') {
				printf("\t");
			} else {
				printf(" ");
			}
		} else {
			printf("~");
		}
	}
}


void print_separator(int from_beginning) {
	if (from_beginning == 0) {
		print_start_line_error();
		print_char_n_times('-', MAX_LINE_LENGTH);
	} else {
		print_char_n_times('-', MAX_LINE_LENGTH + SIDE_LENGTH);
	}

	printf("\n");
}


void set_ending_to_file_name(char *fileName, char ending[]) {
	strcpy(strchr(fileName, '.') + 1, ending);
}