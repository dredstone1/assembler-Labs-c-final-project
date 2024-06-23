#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void print_char_n_times(char c, int n);
void print_error(error error);
void print_start_line_error(bool with_space);
void print_start_line_number_error(int line_number);
int get_word_middle_offset_from_string(int start_word, int word_length, char new_word[]);
int get_word_middle_offset(int start_word, int word_length, int new_word_length);
void print_error_explanation(error error);

const char *error_massage[][2] = {
        {FILE_NOT_FOUND_MESSAGE, ""},
        {MEMORY_ALLOCATION_FAILED_MESSAGE, ""},
        {UNDEFINED_TAG_NAME_MESSAGE, ""},
        {INVALID_OPCODE_MESSAGE, NOT_A_VALID_OPCODE},
        {SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE, },
        {INVALID_COMMA_MESSAGE, MISSING_COMMA_A},
        {MISSING_COMMA_MESSAGE, MISSING_COMMA_A},
        {EXTRA_COMMA_MESSAGE, MISSING_COMMA_A},
        {MISSING_START_QUOTE_MESSAGE, ""},
        {MISSING_ENDING_QUOTE_MESSAGE, ""},
};

void handel_error(error_array error) {
    int marker_length, i, j, error_printed = 0;
    for (i = 0; i < error.size; ++i) {
        marker_length = 0;
        if (error.errors[i].type == NOTHING)
            continue;
        
        if (error_printed > 0)
            printf("------------------------------------------------------------------------------------\n");

        error_printed++;
        print_error(error.errors[i]);
    }
}

void print_error(error error){
    int marker_length=0, j;
    
    print_start_line_error(TRUE);
    printf(FILE_NAME_ERROR, error.file_name);

    
    print_start_line_error(TRUE);
    printf("%s\n", error_massage[error.type][0]);
    print_start_line_error(TRUE);
    printf("\n");
    

    print_error_explanation(error);
    
    printf("%s\n", error_massage[error.type][1]);
}


void print_error_explanation(error error){
    
    print_start_line_number_error(error.line_number);
    printf("%s\n", error.line);
    
    print_start_line_error(TRUE);
    print_char_n_times(' ', error.start_place_in_line);
    
    
    print_char_n_times('~', error.end_place_in_line - error.start_place_in_line);
    printf("\n");
    
    print_char_n_times(' ', get_word_middle_offset_from_string(error.start_place_in_line, error.end_place_in_line , error_massage[error.type][1]));
    printf("%s\n", error_massage[error.type][1]);
    printf("\n");
    
}

int get_word_middle_offset_from_string(int start_word, int word_length, char new_word[]){
    return get_word_middle_offset(start_word, word_length, strlen(new_word));
}

int get_word_middle_offset(int start_word, int word_length, int new_word_length){
    return start_word - abs(new_word_length - word_length) / 2;
}



void print_start_line_error(bool with_space){
    if (with_space)
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
    memcpy(error_array->errors[error_array->size-1].file_name,file_name, strlen(file_name));
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
