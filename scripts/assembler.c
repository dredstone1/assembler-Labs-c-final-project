#include "../header/post_formating.h"
#include "../header/first_pass.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../header/consts.h"

/*
void create_files(word_list_block *file_code_block, symbol_table *table, char file_name[], error_array *error, int IC, int DC);
*/

void run_assembler(char **files_paths, int number_of_files);

int main(int argc, char **argv){
    clock_t tic, toc;
	tic = clock();
    
    run_assembler(argv, argc);
    
    printf(BLACK_COLOR);
    toc = clock();
    printf("time: %f seconds", (double)(toc - tic) / CLOCKS_PER_SEC);
	getchar();
    return 0;
}

void run_assembler(char **files_paths, int number_of_files){
	file file;
	error_array error;
	int i, error_count = 0, IC, DC;
	

	for (i = 1; i < number_of_files; ++i) {
		IC = 0;
		DC = 0;
		initialize_error(&error);
		error.importance = NO_ERROR;
		initialize_new_file_name(&file, &error, files_paths[i]);
		set_ending_to_file_name(file.filename, "as");
/*
		read_file(&file, &error);
*/
		if (error.importance == NO_ERROR) {
/*
			post_formating(&file, &error);
*/
/*
			free_file_lines(&file);
*/
			first_pass(file.filename, &error);
			


		}
		handel_error(error, file.filename);
		error_count += error.size-1;
		free(error.errors);
	}
	printf(BLACK_COLOR);
	printf("total errors: %d\n", error_count);
}

/*
void create_files(word_list_block *file_code_block, symbol_table *table, char file_name[], error_array *error, int IC, int DC){
	if (error->size > 1)
		return;
	write_to_file_object(file_code_block, file_name, IC, DC);
	write_to_file_entry(table, file_name);
	write_to_file_external(file_code_block, file_name, table);
}*/
