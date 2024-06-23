#include "assembler.h"
#include "assembler/post_formating/post_formating.h"
#include "assembler/translation/code_translation.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define BLACK "\033[0m"

void run_assembler(char **files_paths, int number_of_files);

int main(int argc, char **argv){
    clock_t tic = clock();
    
    run_assembler(argv, argc);
    
    printf(BLACK" \n");
    clock_t toc = clock();
    printf("took: %f seconds", (double)(toc - tic) / CLOCKS_PER_SEC);
    return 0;
}

void run_assembler(char **files_paths, int number_of_files){
    file file;
    error_array error;
    int i;

    initialize_error(&error);

    for (i = 1; i < number_of_files; ++i) {
        error.importance = NO_ERROR;
        file.filename = files_paths[i];
        add_ending_to_file_name(&file.filename, &error);
        read_file(&file, &error);
        post_formating(&file, &error);
        translate_code(&file ,&error);
		
		free_file_lines(&file);

		handel_error(error, file.filename);
		free(error.errors);
	}
	while (TRUE){
		if (error.importance == CRITICAL)
			break;
	}
}