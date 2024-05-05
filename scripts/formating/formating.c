#include "formating.h"
#include "../file.h"
#include "../error.h"
#include "line_type.h"

void format_line(file *file1, int line_number, error *error);
void first_pass(file *file1, error *error);

void format_file(file *file1, error *error){
    first_pass(file1, error);

}

void first_pass(file *file1, error *error){
    int i;

    for (i = 0; i < file1->number_of_rows; i++){
        format_line(file1, i, error);
        if (error->error_type != NOTHING)
            return;
    }
}

void format_line(file *file1, int line_number, error *error){
    if (is_empty(file1->line[line_number])){
        return;
    }
}
