#include "error.h"
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_error_count(error error_array);

const char *error_massage[][2] = {
        {FILE_NOT_FOUND_MESSAGE, ""},
        {MEMORY_ALLOCATION_FAILED_MESSAGE, ""},
        {UNDEFINED_TAG_NAME_MESSAGE, ""},
        {INVALID_OPCODE_MESSAGE, NOT_A_VALID_OPCODE},
        {SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE, },
        {INVALID_COMMA_MESSAGE, MISSING_COMMA_A},
        {MISSING_COMMA_MESSAGE, MISSING_COMMA_A},
        {EXTRA_COMMA_MESSAGE, MISSING_COMMA_A},
        {MISSING_START_QUOTE_MESSAGE, MISSING_QUOTE},
        {MISSING_ENDING_QUOTE_MESSAGE, MISSING_QUOTE},
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
        printf("%sError in file: %s\n%d | %s\n%s%s\n%s", start_format_line_error, error.errors[i].file_name, error.errors[i].line_number, error_massage[error.errors[i].type][0], start_format_line_error, error.errors[i].line, start_format_line_error);

        for (j = 0; j < error.errors[i].end_place_in_line; j++) {
            if (j < error.errors[i].start_place_in_line)
                printf(" ");
            else {
                printf("~");
                marker_length++;
            }
        }
        printf("\n%s", start_format_line_error);
        j = 0;
        while (error.errors[i].start_place_in_line >= ((strlen(error_massage[error.errors[i].type][1]) - marker_length-1) / 2) && (j++) < error.errors[i].start_place_in_line -((strlen(error_massage[error.errors[i].type][1]) - marker_length - 1) / 2))
            printf(" ");
        
        printf("%s\n", error_massage[error.errors[i].type][1]);
    }
}

void add_error(error_array *error_array, error_type error_type, int line_number, int start_place_in_line, int end_place_in_line, importance importance, char line[], char file_name[]){
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
    error_array->errors[error_array->size-1].end_place_in_line = end_place_in_line;
    error_array->errors[error_array->size-1].type = error_type;
}

void print_error_pos(int line, int column){
    printf("(");
    print_pos(line, column);
    printf(")");
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
