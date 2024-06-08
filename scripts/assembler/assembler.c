#include "assembler.h"
#include "post_formating/post_formating.h"
#include "first_pass/code_translation.h"

void assembler(file file){
    post_formating(&file);
    translate_code(&file);


    free_file_lines(&file);
}