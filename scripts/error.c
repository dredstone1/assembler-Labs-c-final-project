#include "../header/error.h"
#include <stdio.h>
#include <string.h>

void print_start_line_error();
void print_char_n_times(char c, int n);
void print_start_line_number_error(int line_number);
void print_tilde_and_caret(int start, int end, int pos1, int pos2);

void set_ending_to_file_name(char *fileName, char ending[]);

void print_system_error(char error_massage[]){
	printf("%s\n", error_massage);
}

void print_general_error(char error_massage[], char error_description[], char line[], int line_number, int start_place_in_line, int end_place_in_line, int mark_offset1, int mark_offset2, error *error) {
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}if (error->error_single_file_count > 0)
		print_separator(0);
	
	print_start_line_error();
	printf("%s\n", error_massage);

	print_start_line_number_error(line_number);
	printf("%s\n", line);
	print_start_line_error();
	print_tilde_and_caret(start_place_in_line, end_place_in_line, mark_offset1, mark_offset2);
	
	printf("\n");
	print_start_line_error();
	if (start_place_in_line == 0)
		printf("%.*s%s%s\n", end_place_in_line - start_place_in_line, &line[start_place_in_line], ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE, error_description);
	else
		printf("%.*s%s%s\n", end_place_in_line - start_place_in_line, &line[start_place_in_line-1], ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE, error_description);

	error->error_single_file_count++;
	error->error_count++;
}

void print_start_line_error(){
	print_char_n_times(' ', amount_of_spaces_at_start);
	printf("%s", start_format_line_error);
}

void print_general_error_no_quoting(char error_massage[], char error_description[], char line[], int line_number, int start_place_in_line, int end_place_in_line, int mark_offset1, int mark_offset2, error *error) {
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}if (error->error_single_file_count > 0)
		print_separator(0);

	print_start_line_error();
	printf("%s\n", error_massage);

	print_start_line_number_error(line_number);
	printf("%s\n", line);
	print_start_line_error();
	print_tilde_and_caret(start_place_in_line, end_place_in_line, mark_offset1, mark_offset2);

	printf("\n");
	print_start_line_error();

	printf("%s\n", error_description);

	error->error_single_file_count++;
	error->error_count++;
}

void print_two_line_error(char error_massage[], char error_description[], char line1[], char line2[], int line1_number, int line2_number, int start_place_in_line1, int end_place_in_line1, int start_place_in_line2, int end_place_in_line2, error *error) {
	
	
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}if (error->error_single_file_count > 0)
		print_separator(0);
	
	print_start_line_error();
	printf("%s\n", error_massage);

	print_start_line_number_error(line1_number);
	printf("%s\n", line1);
	print_start_line_error();
	print_tilde_and_caret(start_place_in_line1, end_place_in_line1, 0, 0);
	printf("\n");

	print_start_line_number_error(line2_number);
	printf("%s\n", line2);
	print_start_line_error();
	print_tilde_and_caret(start_place_in_line2, end_place_in_line2, 0, 0);

	printf("\n");
	print_start_line_error();

	if (start_place_in_line1 == 0) {
		printf("%.*s%s%s", end_place_in_line1 - start_place_in_line1, &line1[start_place_in_line1], ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE, error_description);
	} else {
		printf("%.*s%s%s", end_place_in_line1 - start_place_in_line1, &line1[start_place_in_line1 - 1], ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE, error_description);
	}
	printf(" %d\n", line1_number);

	error->error_single_file_count++;
	error->error_count++;
}

void print_macro_b_label_same(char line[], int line_number, int macro_line_number, error *error, char macro[]) {
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}if (error->error_single_file_count > 0)
		print_separator(0);

	print_start_line_error();
	printf("%s\n", SYMBOL_CAN_BE_SAME_AS_MACRO_NAME);

	print_start_line_number_error(line_number);
	printf("%s\n", line);

	print_start_line_error();
	print_tilde_and_caret(strstr(line, macro) - line + 1, strstr(line, macro) - line + strlen(macro) + 1, 0, 0);
	
	printf("\n");
	print_start_line_error();
	printf("%.*s%s%s\n", (int)strlen(macro), macro, ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE, SYMBOL_CAN_BE_SAME_AS_MACRO_NAME_DESCRIPTION);
	
	print_start_line_error();
	set_ending_to_file_name(error->file_name, "as");
	printf(SYMBOL_ALREADY_EXISTS_AS_MACRO_DESCRIPTION, error->file_name, macro_line_number+1);
	printf("\n");
	set_ending_to_file_name(error->file_name, "am");
	
	error->error_single_file_count++;
	error->error_count++;
}


void print_command_not_legal(char variable[], char line[], int line_number, error *error,const char variable_type_valid[], int usage, opcode code){
	int legal_printed = 0, i;
	
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}if (error->error_single_file_count > 0)
		print_separator(0);

	print_start_line_error();
	printf("%s\n", INVALID_VARIABLE_TYPE_MESSAGE);

	print_start_line_number_error(line_number);
	printf("%s\n", line);

	print_start_line_error();
	print_tilde_and_caret(strstr(line, variable) - line+1, strstr(line, variable) - line + strlen(variable)+1, 0, 0);
	printf("\n");

	print_start_line_error();
	if (usage == 0) {
		printf("%s %s", variable, INVALID_VARIABLE_TYPE_SOURCE_DESCRIPTION);
	}
	else {
		printf("%s %s", variable, INVALID_VARIABLE_TYPE_DESTINATION_DESCRIPTION);
	}

	printf("%s %s", opcode_names[code][0], LEGAL_VARIABLE_TYPES_MESSAGE);

	for (i = 0; i < 5; i++) {
		if (variable_type_valid[i] != '_') {
			if (legal_printed > 0)
				printf(", ");
			printf("%s", operand_names[i]);
			legal_printed++;
		}
	}

	printf("\n");
}

void print_start_line_too_long_error(int line_number, error *error){
	if (error->error_single_file_count == 0) {
		printf(FILE_NAME_ERROR, error->file_name);
		print_separator(1);
	}if (error->error_single_file_count > 0)
		print_separator(0);

	print_start_line_error();
	printf("%s\n", LINE_TOO_LONG_MESSAGE);

	print_start_line_number_error(line_number);
}

void print_end_line_too_long_error() {
	print_start_line_error();
	printf("%s\n", LINE_TOO_LONG_DESCRIPTION);
}

void print_char_n_times(char c, int n){
	int i;
	for (i = 0; i < n; i++)
		printf("%c", c);
}

void print_start_line_number_error(int line_number){
	printf("%4d | ", line_number);
}

void print_tilde_and_caret(int start, int end, int pos1, int pos2) {
	if (start > MAX_LINE_LENGTH)
		return;
	print_char_n_times(' ', start-1);
	if (pos1 == 0)
		print_char_n_times('~', (end - start));
	else{
		print_char_n_times('~',pos1 - start);
		printf("^");
		if (pos2 == 0 || pos2 == pos1)
			print_char_n_times('~', (end - pos1));
		else{
			print_char_n_times('~', (pos2 - pos1)-1);
			printf("^");
			print_char_n_times('~', (end - pos2));
		}
		
	}
}

void print_separator(int from_beginning){
	if (from_beginning == 0) {
		print_start_line_error();
		print_char_n_times('-', MAX_LINE_LENGTH);
	}else
		print_char_n_times('-', MAX_LINE_LENGTH + SIDE_LENGTH);
	printf("\n");
}

void set_ending_to_file_name(char *fileName, char ending[]){
	strcpy(strchr(fileName, '.') + 1, ending);
}