#include "assembler.h"
#include "assembler/post_formating/post_formating.h"
#include "assembler/translation/code_translation.h"

void run_assembler(char **files_paths, int number_of_files);
int main(int argc, char **argv){
    run_assembler(argv, argc);

    return 0;
}

void run_assembler(char **files_paths, int number_of_files){
    file file;
    error error;
    error.error_type = NOTHING;
    int i;

    for (i = 1; i < number_of_files; ++i) {
        file.filename = files_paths[i];
        read_file(&file, &error);
        post_formating(&file);
        translate_code(&file);
        free_file_lines(&file);
    }
}