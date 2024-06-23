#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_char_n_times(char c, int n);
void print_error(error error);
void print_start_line_error();
void print_start_line_number_error(int line_number);
void print_separator();
int count_error_type(error_array error_array, int current_error, error_type error_type);
void print_2_errors(error error1, error *error2);
void print_errors(error error);
int get_next_error_by_type(error_array error_array, int current_error, error_type error_type);

#define RED "\033[31m" // Set text color to red

void print_general_error_type(error error);
void print_error_line(error error);
void print_error_mark(error error);
void print_error_mark_addition(error error);

const error_message_stage error_massage_stages_1_error[][END_OF_ERROR+1] = {
/*FILE_NOT_FOUND_MESSAGE: */ {PRINT_GENERAL_ERROR_TYPE, END_OF_ERROR},
/*MEMORY_ALLOCATION_FAILED_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, END_OF_ERROR},
/*UNDEFINED_TAG_NAME_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*INVALID_OPCODE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*INVALID_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*MISSING_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*EXTRA_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*MISSING_START_QUOTE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*MISSING_ENDING_QUOTE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*DIRECTIVE_TYPE_MISSING_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
};

const error_message_stage error_massage_stages_2_errors[][END_OF_ERROR+1] = {
/*FILE_NOT_FOUND_MESSAGE: */ {PRINT_GENERAL_ERROR_TYPE, END_OF_ERROR},
/*MEMORY_ALLOCATION_FAILED_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, END_OF_ERROR},
/*UNDEFINED_TAG_NAME_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*INVALID_OPCODE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*INVALID_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*MISSING_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*EXTRA_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*MISSING_START_QUOTE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*MISSING_ENDING_QUOTE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*DIRECTIVE_TYPE_MISSING_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
};

const error_message_stage error_massage_stages_errors[][END_OF_ERROR+1] = {
/*FILE_NOT_FOUND_MESSAGE: */ {PRINT_GENERAL_ERROR_TYPE, END_OF_ERROR},
/*MEMORY_ALLOCATION_FAILED_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, END_OF_ERROR},
/*UNDEFINED_TAG_NAME_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*INVALID_OPCODE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*INVALID_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*MISSING_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*EXTRA_COMMA_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*MISSING_START_QUOTE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*MISSING_ENDING_QUOTE_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
/*DIRECTIVE_TYPE_MISSING_MESSAGE*/ {PRINT_GENERAL_ERROR_TYPE, PRINT_ERROR_LINE, PRINT_ERROR_MARK, PRINT_ERROR_MARK_ADDITION, END_OF_ERROR},
};

const char *error_massage[][2] = {
        {FILE_NOT_FOUND_MESSAGE, " "},
        {MEMORY_ALLOCATION_FAILED_MESSAGE, " "},
        {UNDEFINED_TAG_NAME_MESSAGE, " "},
        {INVALID_OPCODE_MESSAGE, NOT_A_VALID_OPCODE},
        {SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE, " "},
        {INVALID_COMMA_MESSAGE, MISSING_COMMA_A},
        {MISSING_COMMA_MESSAGE, MISSING_COMMA_A},
        {EXTRA_COMMA_MESSAGE, MISSING_COMMA_A},
        {MISSING_START_QUOTE_MESSAGE, MISSING_QUOTE},
        {MISSING_ENDING_QUOTE_MESSAGE, MISSING_QUOTE},
        {DIRECTIVE_TYPE_MISSING_MESSAGE, ""},
};

void (*error_handling_functions[])(error) = {
        print_general_error_type,
        print_error_line,
        print_error_mark,
        print_error_mark_addition
};


void handel_error(error_array error) {
    int i, error_printed = 0, temp_count;

    printf(RED);
    for (i = 0; i < error.size; ++i) {
        if (error.errors[i].type == NOTHING)
            continue;
        
        if (error_printed > 0)
			print_separator();
		
		
		temp_count = count_error_type(error, i, error.errors[i].type);
		if (temp_count == 1){
			print_error(error.errors[i]);
		}else if (temp_count == 2){
			print_2_errors(error.errors[i], &error.errors[get_next_error_by_type(error, i+1, error.errors[i].type)]);
		}else {
			print_errors(error.errors[i]);
		}
		
        error_printed++;
    }
}

void print_2_errors(error error1, error *error2){
	int i;
	printf("2 errors\n");
	for (i = 0; error_massage_stages_2_errors[error1.type][i] != END_OF_ERROR; ++i)
		error_handling_functions[error_massage_stages_1_error[error1.type][i]](error1);
	
	for (i = 0; error_massage_stages_2_errors[error2->type][i] != END_OF_ERROR; ++i)
		error_handling_functions[error_massage_stages_1_error[error2->type][i]](*error2);
	
	error2->type = NOTHING;
}

void print_errors(error error){
	int i;
	printf("3+ errors\n");
	for (i = 0; error_massage_stages_2_errors[error.type][i] != END_OF_ERROR; ++i)
		error_handling_functions[error_massage_stages_2_errors[error.type][i]](error);
}

void print_error(error error){
    int i;

	printf("1 error\n");
    for (i = 0; error_massage_stages_errors[error.type][i] != END_OF_ERROR; ++i)
        error_handling_functions[error_massage_stages_errors[error.type][i]](error);
}

void print_start_line_error(){
    print_char_n_times(' ', amount_of_spaces_at_start);
    printf("%s", start_format_line_error);
}

void print_start_line_number_error(int line_number){
    printf("%4d | ", line_number);
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

void print_in_error_message(error error){
    print_start_line_error();
    printf(FILE_NAME_ERROR, error.file_name);
}

void print_general_error_type(error error){
    print_start_line_error();
    printf("%s\n", error_massage[error.type][0]);
}

void print_error_line(error error){
    print_start_line_number_error(error.line_number);
    printf("%s\n", error.line);
}

void print_error_mark(error error){
    print_start_line_error();
	print_char_n_times(' ', error.start_place_in_line);
	
	if (error.mark_offset > 0) {
		print_char_n_times('~', error.mark_offset - error.start_place_in_line);
		printf("^");
		print_char_n_times('~', error.end_place_in_line - error.mark_offset - 1);
		printf("\n");
	}else
		print_char_n_times('~', error.end_place_in_line - error.start_place_in_line);
    printf("\n");
}

int count_error_type(error_array error_array, int current_error, error_type error_type){
	int i, count = 0;
	for (i = current_error; i < error_array.size; i++) {
		if (error_array.errors[i].type == error_type)
			count++;
	}
	return count;

}

int get_next_error_by_type(error_array error_array, int current_error, error_type error_type){
	int i;
	for (i = current_error; i < error_array.size; i++) {
		if (error_array.errors[i].type == error_type)
			return i;
	}
	return -1;
}












void add_error(error_array *error_array, error_type error_type, int line_number, int start_place_in_line, int end_place_in_line, importance importance, char line[], char file_name[], int mark_offset){
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
	error_array->errors[error_array->size-1].file_name = file_name;
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
    error_array->errors->type = NOTHING;
    
    error_array->size = 1;
    if (error_array->errors == NULL) {
        printf("Memory allocation failed");
        return;
    }
}
