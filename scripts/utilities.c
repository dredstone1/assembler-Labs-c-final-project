#include "../header/utilities.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

int is_separator(char c, char separators[]);




int int_to_octal(int decimal_number) {
	int octal_result = 0;
	int place_value = 1;

	while (decimal_number != 0) {
		octal_result += (decimal_number % 8) * place_value;
		decimal_number /= 8;
		place_value *= 10;
	}

	return octal_result;
}




int count_commas_until_text(char **workable_line) {
	int count = 0;
	while ( **workable_line == ' ' || **workable_line == '\t' || **workable_line == ',') {
		if (**workable_line == ',')
			count++;
		(*workable_line)++;
	}
	return count;
}

int get_next_word(char **workable_line, char word[], char separators[]) {
	int i = 0;
	while (is_separator(**workable_line, separators) == 0 && **workable_line != '\0'){
		word[i++] = **workable_line;
		(*workable_line)++;
	}
	word[i] = '\0';
	return i;
}

char* search_for_next_white_char(char *line) {
	while (*line != ' ' && *line != '\t' && *line != '\0' && *line != '\n' && *line != '\r')
		line++;
	
	return line;
}

int is_separator(char c, char separators[]) {
	int i;
	for (i = 0; i < strlen(separators); i++)
		if (c == separators[i])
			return 1;

	return 0;
}

int extra_char_at_end(char *line) {
	skip_spaces_and_tabs(&line);

	if (*line == '\0')
		return 0;

	return 1;
}


int is_empty_line(char line[]){
	int offset = 0;
	skip_spaces_and_tabs_with_offset(&offset, line);

	return line[offset] == '\0' || line[offset] == '\n' || line[offset] == '\r';
}

int is_comment_line(char line[]){
	int offset = 0;
	skip_spaces_and_tabs_with_offset(&offset, line);

	return line[offset] == ';';
}

void skip_spaces_and_tabs_with_offset(int *offset, char line[]) {
	while (line[*offset] == ' ' || line[*offset] == '\t')
		(*offset)++;
}

void skip_spaces_and_tabs(char **line) {
	while (**line == ' ' || **line == '\t')
		(*line)++;
}


void *use_malloc(size_t size, error *error) {
	void *ptr = (void *) malloc(size);
	if (ptr == NULL){
		print_system_error(MEMORY_ALLOCATION_FAILED_MESSAGE);
		error->importance = CRITICAL;
	}

	return ptr;
}

void *use_realloc(void *ptr, size_t size, error *error) {
	if (ptr == NULL)
		ptr = use_malloc(size, error);
	else
		ptr = realloc(ptr, size);
	
	if (ptr == NULL){
		print_system_error(MEMORY_ALLOCATION_FAILED_MESSAGE);
		error->importance = CRITICAL;
	}
	
	return ptr;
}

int is_register(const char word[]) {
	int offset = 0;
	if (word[0] == '*')
		offset = 1;

	if (word[offset] == 'r' && word[offset+1] >= '0' && word[offset+1] <= '9' && word[offset+2] == '\0')
		return 1;

	return 0;
}



int get_line_from_file(char line[], int line_number, char file_name[], error *error) {
	FILE *file = fopen(file_name, "r");
	if (file == NULL) {
		print_system_error(FILE_NOT_FOUND_MESSAGE);
		error->importance = CRITICAL;
		return 0;
	}

	while (line_number > 0 && fgets(line, MAX_LINE_LENGTH, file) != NULL)
		line_number--;

	if (line[strlen(line) - 1] == '\n'|| line[strlen(line) - 1] == '\r')
		line[strlen(line) - 1] = '\0';

	fclose(file);
	return 1;
}

void go_to_next_line(FILE *file, char line[], int line_number, int *current_line_number){
	for (; *current_line_number < line_number; (*current_line_number)++)
		fgets(line, MAX_LINE_LENGTH, file);

	if (line[strlen(line) - 1] == '\n' || line[strlen(line) - 1] == '\r')
		line[strlen(line) - 1] = '\0';
}

int is_a_saved_word(const char word[]){
	if (get_opcode_from_string(word) != -1)
		return 0;

	if (get_instruction_type_from_string(word) != -1)
		return 1;

	if (is_register(word) == 1)
		return 2;

	if (strcmp(word, MACRO) == 0 || strcmp(word, END_MACRO) == 0)
		return -2;

	return 3;
}

directive_type get_instruction_type_from_string(const char *str){
	directive_type instruction;

	for (instruction = DATA; instruction <= EXTERN; instruction++)
		if (strcmp(str, instruction_type_names[instruction]) == 0)
			return instruction;
	return -1;
}

opcode get_opcode_from_string(const char *str) {
	opcode code;
	for (code = MOV; code <= STOP; code++)
		if (strcmp(str, opcode_names[code][0]) == 0)
			return code;

	return -1;
}

char* duplicate_string(const char *source, error *error) {
	char *destination = (char *) use_malloc((strlen(source)+1) * sizeof(char), error);
	if (destination == NULL) {
		error->importance = CRITICAL;
		print_system_error(MEMORY_ALLOCATION_FAILED_MESSAGE);
		return NULL;
	}
	strcpy(destination, source);
	return destination;
}

void replace_line_ending(char line[]) {
	char *line_temp;
	line_temp = strchr(line, '\r');
	if (line_temp != NULL)
		*line_temp = '\0';

	line_temp = strchr(line, '\n');
	if (line_temp != NULL)
		*line_temp = '\0';
}

int is_line_too_long(const char line[]) {
	int i = 0;
	while (line[i] != '\0' && line[i] != '\n' && line[i] != '\r' && i < MAX_LINE_LENGTH)
		i++;
	return i < (MAX_LINE_LENGTH-2);
}

