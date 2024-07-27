#include "../header/utilities.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int is_separator(char c, char separators[]);

const char *opcode_names[16][3][1] = {
		{{"mov"}, {"_123"}, {"0123"}},
		{{"cmp"}, {"0123"}, {"0123"}},
		{{"add"}, {"_123"}, {"0123"}},
		{{"sub"}, {"_123"}, {"0123"}},
		{{"lea"}, {"_123"}, {"_1__"}},
		{{"clr"}, {"_123"}, {"____"}},
		{{"not"}, {"_123"}, {"____"}},
		{{"inc"}, {"_123"}, {"____"}},
		{{"dec"}, {"_123"}, {"____"}},
		{{"jmp"}, {"_12_"}, {"____"}},
		{{"bne"}, {"_12_"}, {"____"}},
		{{"red"}, {"_123"}, {"____"}},
		{{"prn"}, {"0123"}, {"____"}},
		{{"jsr"}, {"_12_"}, {"____"}},
		{{"rts"}, {"____"}, {"____"}},
		{{"stop"}, {"____"}, {"____"}}
};


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

int is_separator(char c, char separators[]) {
	int i;
	for (i = 0; i < strlen(separators); i++)
		if (c == separators[i])
			return 1;

	return 0;
}

int extra_char_at_end(const char line[], int loc){
	while (line[loc] == ' ' || line[loc] == '\t')
		loc++;

	if (line[loc] == '\0')
		return 0;

	return 1;
}


int is_empty_line(char line[]){
	int offset = 0;
	skip_spaces_and_tabs_with_offset(&offset, line);

	return line[offset] == '\0' || line[offset] == '\n';
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


void *use_malloc(size_t size, error_array *error) {
	void *ptr = (void*)malloc(size);
	if (ptr == NULL)
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0, 0);

	return ptr;
}

void *use_realloc(void *ptr, size_t size, error_array *error) {
	if (ptr == NULL)
		ptr = use_malloc(size, error);
	else
		ptr = realloc(ptr, size);
	
	if (ptr == NULL)
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0, 0);

	return ptr;
}

void free_lines(line_text *lines, int number_of_lines) {
	int i;
	for (i = 0; i < number_of_lines; i++)
		if(lines[i].allocated)
			free(lines[i].content);

	free(lines);
}