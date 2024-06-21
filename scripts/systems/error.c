#include "error.h"
#include "file.h"
#include <stdio.h>
#include <stdlib.h>

int get_error_count(error error_array);

const char **error_massage{
    FILE_NOT_FOUND_MESSAGE,
    FILE_NOT_OPEN_MESSAGE,
    MEMORY_ALLOCATION_FAILED_MESSAGE,
    NOTHING_MESSAGE,
    INVALID_LABEL_MESSAGE,
    INVALID_OPCODE_MESSAGE,
    INVALID_DIRECTIVE_MESSAGE,
    INVALID_REGISTER_MESSAGE,
    INVALID_NUMBER_MESSAGE,
    INVALID_COMMA_MESSAGE,
    MISSING_COMMA_MESSAGE,
    EXTRA_COMMA_MESSAGE,
    MISSING_START_QUOTE_MESSAGE,
    MISSING_ENDING_QUOTE_MESSAGE,
    SYMBOL_IN_EXTERNAL_OR_ENTRY_MESSAGE,
    UNDEFINED_TAG_NAME_MESSAGE,
    
    
};
void handel_error(error_array error){
    for (int i = 0; i < error.size; ++i) {
        printf("%3d | %s\n", lineNumber, codeSnippet);
        printf("      | %*s^~~~~\n", 10 + (int)strlen(codeSnippet), "");
        printf("      | %s\n", errorMessage);
    }
    
}

void printError(const char* errorMessage, int lineNumber, const char* codeSnippet) {
    printf("%3d | %s\n", lineNumber, codeSnippet);
    printf("      | %*s^~~~~\n", 10 + (int)strlen(codeSnippet), "");
    printf("      | %s\n", errorMessage);
}


void add_error(error_array *error_array, error_type error_type, int line_number, int start_place_in_line, int end_place_in_line, importance importance) {
    error *error_array_temp;
    error_array_temp = (error *)realloc(error_array->errors, sizeof(error) * (error_array->size+1));
    if (error_array_temp != NULL) {
        error_array->errors = error_array_temp;
        error_array->size++;
    }
    else {
        error_type = MEMORY_ALLOCATION_FAILED;
    }
    
    error_array->importance = importance;
    
    error_array->errors[error_array->size-1].line_number = line_number;
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
