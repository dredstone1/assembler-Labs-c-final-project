#include "assembler.h"
#include "post_formating/post_formating.h"
#include "first_pass/first_pass.h"

void assembler(file file){
    post_formating(&file);
    first_pass(&file);

    free_file_lines(&file);
}