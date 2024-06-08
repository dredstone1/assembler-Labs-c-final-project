#include "../../systems/file.h"
#include "macro.h"

void set_line_type(line_node *node);

void post_formating(file *file){
    macros macros;
    handle_macros(&file->first_line, file->number_of_rows, &macros);
    write_to_file_file(*file);
    free_macros(&macros);
}

