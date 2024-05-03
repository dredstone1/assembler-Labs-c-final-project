#include "formating.h"
#include "../file.h"
#include "../error.h"

void format_line(file *file1, int line_number, error *error);

void format_file(file *file1, error *error){
    int i;

    for (i = 0; i < file1->number_of_rows; i++){
        format_line(file1, i, error);
        if (error->error_type != NOTHING)
            return;
    }
}

void format_line(file *file1, int line_number, error *error){

}