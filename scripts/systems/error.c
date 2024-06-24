#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_char_n_times(char c, int n);
void print_error(error error, error_array error_array, int current_error);
void print_start_line_error();
void print_start_line_number_error(int line_number);
void print_separator();
int count_error_type(error_array error_array, int current_error, error_type error_type);
int get_next_error_by_type(error_array error_array, int current_error, error_type error_type);
void print_in_error_message(char file_name[]);
void print_start_error_message(error error, int *offset, bool single_error);
void print_error_lines(error error, int *offset, bool ignore_mark, bool single_error);

/*Set text color to red*/
#define RED "\x1b[38;2;247;84;100m"

void print_general_error_type(error error, bool single_error);
void print_error_line(error error, bool single_error);
void print_error_mark(error error, bool single_error);
void print_error_message_conclusion(error error, bool single_error);

const error_message_stage error_massage_stage[][END_OF_ERROR+3] = {
/*FILE_NOT_FOUND_MESSAGE: */ {FOR_EVERY_ERROR, PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, END_OF_ERROR},
/*MEMORY_ALLOCATION_FAILED_MESSAGE*/ {FOR_EVERY_ERROR, PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, END_OF_ERROR},
/*UNDEFINED_TAG_NAME_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, PRINT_ERROR_LINE, PRINT_ERROR_MARK, FOR_EVERY_ERROR, END_OF_ERROR},
/*INVALID_OPCODE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, PRINT_ERROR_LINE, PRINT_ERROR_MARK, FOR_EVERY_ERROR, PRINT_ERROR_MESSAGE_CONCLUSION, END_OF_ERROR},
/*SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, PRINT_ERROR_LINE, PRINT_ERROR_MARK, FOR_EVERY_ERROR, PRINT_ERROR_MESSAGE_CONCLUSION, END_OF_ERROR},
/*INVALID_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, PRINT_ERROR_LINE, PRINT_ERROR_MARK, FOR_EVERY_ERROR, PRINT_ERROR_MESSAGE_CONCLUSION, END_OF_ERROR},
/*MISSING_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, PRINT_ERROR_LINE, PRINT_ERROR_MARK, FOR_EVERY_ERROR, PRINT_ERROR_MESSAGE_CONCLUSION, END_OF_ERROR},
/*EXTRA_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, PRINT_ERROR_LINE, PRINT_ERROR_MARK, FOR_EVERY_ERROR, PRINT_ERROR_MESSAGE_CONCLUSION, END_OF_ERROR},
/*MISSING_START_QUOTE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, PRINT_ERROR_LINE, PRINT_ERROR_MARK, FOR_EVERY_ERROR, PRINT_ERROR_MESSAGE_CONCLUSION, END_OF_ERROR},
/*MISSING_ENDING_QUOTE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, PRINT_ERROR_LINE, PRINT_ERROR_MARK, FOR_EVERY_ERROR, PRINT_ERROR_MESSAGE_CONCLUSION, END_OF_ERROR},
/*MISSING_ENDING_QUOTE_N_START_QUOTE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, PRINT_ERROR_LINE, PRINT_ERROR_MARK, FOR_EVERY_ERROR, PRINT_ERROR_MESSAGE_CONCLUSION, END_OF_ERROR},
/*DIRECTIVE_TYPE_MISSING_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, FOR_EVERY_ERROR, PRINT_ERROR_LINE, PRINT_ERROR_MARK, FOR_EVERY_ERROR, PRINT_ERROR_MESSAGE_CONCLUSION, END_OF_ERROR},
};

const char *error_massage[][2] = {
        {FILE_NOT_FOUND_MESSAGE},
        {MEMORY_ALLOCATION_FAILED_MESSAGE},
        {UNDEFINED_TAG_NAME_MESSAGE},
        {INVALID_OPCODE_MESSAGE, INVALID_OPCODE_DESCRIPTION},
        {SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE, SYMBOL_IN_EXTERNAL_OR_ENTRY_DESCRIPTION},
        {INVALID_COMMA_MESSAGE, INVALID_COMMA_DESCRIPTION},
        {MISSING_COMMA_MESSAGE, MISSING_COMMA_DESCRIPTION},
        {EXTRA_COMMA_MESSAGE, EXTRA_COMMA_DESCRIPTION},
        {MISSING_START_QUOTE_MESSAGE, MISSING_START_QUOTE_DESCRIPTION},
        {MISSING_ENDING_QUOTE_MESSAGE, MISSING_ENDING_QUOTE_DESCRIPTION},
        {MISSING_ENDING_QUOTE_N_START_QUOTE_MESSAGE, MISSING_ENDING_QUOTE_N_START_QUOTE_DESCRIPTION},
        {DIRECTIVE_TYPE_MISSING_MESSAGE, DIRECTIVE_TYPE_MISSING_DESCRIPTION},
};

void (*error_handling_functions[])(error, bool) = {
        print_general_error_type,
        print_error_line,
        print_error_mark,
		print_error_message_conclusion
};


void handel_error(error_array error, char file_name[]) {
    int i, error_printed = 0;

    printf(RED);
    for (i = 0; i < error.size; ++i) {
        if (error.errors[i].type == NOTHING)
            continue;
        
        if (error_printed > 0)
			print_separator();
		else
			print_in_error_message(file_name);
		
		print_error(error.errors[i], error, i);
		
        error_printed++;
    }
}


void print_error(error error, error_array error_array, int current_error){
	int count = count_error_type(error_array, current_error, error.type), i=0, j, temp;

	print_start_error_message(error, &i, FALSE);
	i++;
	temp = i;
	for (j = 0; j < count; ++j) {
		i = temp;
		
		print_error_lines(error, &i, (count == 2  && j==1) || (count != 2 ), count == 2);
		error_array.errors[current_error].type = NOTHING;
		
		current_error = get_next_error_by_type(error_array, current_error, error.type);
		if (current_error == -1)
			break;

		error = error_array.errors[current_error];
	}
}

void print_error_lines(error error, int *offset, bool ignore_mark, bool single_error){
	for (; error_massage_stage[error.type][*offset] != END_OF_ERROR; ++(*offset)) {
		if (error_massage_stage[error.type][*offset] == FOR_EVERY_ERROR){
			if (ignore_mark == TRUE)
				continue;
			return;
		}
			
		error_handling_functions[error_massage_stage[error.type][*offset]](error, single_error);
	}
}

void print_start_error_message(error error, int *offset, bool single_error){
	for (; error_massage_stage[error.type][*offset] < END_OF_ERROR; (*offset)++)
		error_handling_functions[error_massage_stage[error.type][*offset]](error, single_error);
}



void print_start_line_error(){
    print_char_n_times(' ', amount_of_spaces_at_start);
    printf("%s", start_format_line_error);
}

void print_start_line_number_error(int line_number){
    printf("%4d | ", line_number+1);
}

void print_char_n_times(char c, int n){
    int i;
    for (i = 0; i < n; i++)
        printf("%c", c);
}

void print_separator(){
    print_start_line_error();
    print_char_n_times('-', LINE_SIZE);
    printf("\n");
}

void print_in_error_message(char file_name[]){
    printf(FILE_NAME_ERROR, file_name);
}

void print_general_error_type(error error, bool single_error){
    print_start_line_error();
    printf("%s\n", error_massage[error.type][0]);
}

void print_error_message_conclusion(error error, bool single_error){
    print_start_line_error();
	if (single_error == FALSE) {
		printf("%.*s", error.end_place_in_line - error.start_place_in_line, &error.line[error.start_place_in_line]);
		printf(ERROR_DESCRIPTION_MESSAGE_START_PATTERN_SINGLE);
		printf("%s", error_massage[error.type][1]);
	} else{
		printf("%.*s", error.end_place_in_line - error.start_place_in_line, &error.line[error.start_place_in_line]);
		printf(ERROR_DESCRIPTION_MESSAGE_START_PATTERN_DOUBLE);
		printf("%.*s", error.end_place_in_line - error.start_place_in_line, &error.line[error.start_place_in_line]);
		printf("%s", error_massage[error.type][1]);
	}
	printf("\n");
		
}

void print_error_line(error error, bool single_error){
    print_start_line_number_error(error.line_number);
    printf("%s\n", error.line);
}

void print_error_mark(error error, bool single_error){
    print_start_line_error();
	print_char_n_times(' ', error.start_place_in_line);
	
	if (error.mark_offset > 0) {
		print_char_n_times('~', error.mark_offset - error.start_place_in_line);
		printf("^");
		print_char_n_times('~', error.end_place_in_line - error.mark_offset - 1);
	}else
		print_char_n_times('~', error.end_place_in_line - error.start_place_in_line);
	
	if (error.type == MISSING_ENDING_QUOTE_N_START_QUOTE)
		printf("^");

    printf("\n");
}

int count_error_type(error_array error_array, int current_error, error_type error_type){
	int i, count = 0;
	for (i = current_error; i < error_array.size; i++)
		if (error_array.errors[i].type == error_type)
			count++;

	return count;

}

int get_next_error_by_type(error_array error_array, int current_error, error_type error_type) {
	int i;
	for (i = current_error + 1; i < error_array.size; i++) {
		if (error_array.errors[i].type == error_type)
			return i;
	}
	return -1;
}

void add_error(error_array *error_array, error_type error_type, int line_number, int start_place_in_line, int end_place_in_line, importance importance, char line[], int mark_offset) {
    error *error_array_temp;
    error_array_temp = (error *)realloc(error_array->errors, sizeof(error) * (error_array->size+1));
    if (error_array_temp != NULL) {
        error_array->errors = error_array_temp;
        error_array->size++;
    }
    else
        error_type = MEMORY_ALLOCATION_FAILED;
    
    error_array->importance = importance;
    error_array->errors[error_array->size-1].line_number = line_number;
    memcpy(error_array->errors[error_array->size-1].line,line, strlen(line));
    error_array->errors[error_array->size-1].start_place_in_line = start_place_in_line;
    error_array->errors[error_array->size-1].mark_offset = mark_offset;
    error_array->errors[error_array->size-1].end_place_in_line = end_place_in_line;
    error_array->errors[error_array->size-1].type = error_type;
}

void initialize_error(error_array *error_array){
    error_array->errors = (error*)malloc(sizeof(error));
    error_array->errors->line_number = 0;
    error_array->errors->start_place_in_line = 0;
    error_array->errors->end_place_in_line = 0;
	error_array->errors->mark_offset = 0;
	error_array->errors->line[0] = '\0';
    error_array->errors->type = NOTHING;
	error_array->importance = NO_ERROR;
    
    error_array->size = 1;
    if (error_array->errors == 
	NULL) {
        printf("Memory allocation failed");
        return;
    }
}
