#include "../header/file.h"
#include "../header/macro.h"

void post_formating(file *file, error_array *error){
    macros macros;
    handle_macros(&file->first_line, file->number_of_rows, &macros, error);
    write_to_file_file(*file);
    free_macros(&macros);
}

