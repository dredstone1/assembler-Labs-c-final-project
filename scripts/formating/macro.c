#include "macro.h"
#include <stdio.h>

bool is_line_macro(const char line[], pos *pos){
    for (pos->column = 0; 6 > pos->column; pos->column++){
        printf("eeee1: %d\n", pos->column);
        printf("eeee2: %s\n\n", line);
        if(line[pos->column] == '\0')
            return FALSE;
        if (line[pos->column] != MACRO[pos->column])
            return FALSE;
    }

    return TRUE;
}

void set_macro_name(const char line[], macro *macro, pos *pos){
    bool found_text = FALSE;

    for (; pos->column<LINE_SIZE; pos->column++){
        if (line[pos->column]==' '|| line[pos->column]=='\t'){
            if (!found_text){
                continue;
            }

            /*error extra text after the macro name*/
        } else if (line[pos->column] == '\n' || line[pos->column] == '\0'){
            if(found_text){
                macro->macro_name[pos->column] = '\0';
                break;
            }

            /*error undefined macro name*/
        } else {
            macro->macro_name[pos->column] = line[pos->column];
            found_text = TRUE;
        }
    }
}
/*

void get_macro_lines(file *file1, error *error){
    int i,line_number=0;
    size_t lenmax = sizeof(line) * LINE_JUMPER_SIZE;

    file1->pos->column = 0;
    i = file1->pos->line+1;

    file1->macros->macro_lines = malloc(0);

    for (; i < file1->number_of_rows; i++){
        if (strcmp(file1->line[i].content, "endmacr")==0)
            break;

        lenmax += sizeof(line) * LINE_JUMPER_SIZE;
        file1->macros->macro_lines = realloc(file1->macros->macro_lines, sizeof(macro));

        if (file1->line == NULL) {
            error->error_type = MEMORY_ALLOCATION_FAILED;
            return;
        }
        file1->macros[macro_lines].line_number = line_number;
        line_number++;
    }
}*/
