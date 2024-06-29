#include "../header/post_formating.h"
#include "../header/first_pass.h"
#include <stdlib.h>
#include <stdio.h>

void create_files(word_list_block *file_code_block, symbol_table *table, char file_name[], error_array *error);

void run_assembler(char **files_paths, int number_of_files);

int main(int argc, char **argv){
/*
    clock_t tic, toc;
	tic = clock();
*/
    
    run_assembler(argv, argc);
    
    printf(BLACK_COLOR);
/*
    toc = clock();
*/
/*
    printf("time: %f seconds", (double)(toc - tic) / CLOCKS_PER_SEC);
*/
	getchar();
    return 0;
}

void run_assembler(char **files_paths, int number_of_files){
	file file;
	error_array error;
	int i, error_count = 0;
	word_list_block *file_code_block;
	symbol_table table;
	

	for (i = 1; i < number_of_files; ++i) {
		initialize_error(&error);
		error.importance = NO_ERROR;
		initialize_new_file_name(&file, &error, files_paths[i]);
		read_file(&file, &error);
		if (error.importance == NO_ERROR) {
			post_formating(&file, &error);
			file_code_block = create_new_word_list_block(&error);
			initialize_symbol_table(&table);
			first_pass(&file, &table, file_code_block, &error);

			add_symbols_to_code_block(file_code_block, &table, &error);
			create_files(file_code_block, &table, file.filename, &error);

			free_word_list_block(file_code_block);

			free_file_lines(&file);
		}
		handel_error(error, file.filename);
		error_count += error.size-1;
		free(error.errors);
	}
	printf(BLACK_COLOR);
	printf("total errors: %d\n", error_count);
}

void create_files(word_list_block *file_code_block, symbol_table *table, char file_name[], error_array *error){
	if (error->size > 1)
		return;
	write_to_file_object(file_code_block, file_name);
	write_to_file_entry(table, file_name);
	write_to_file_external(file_code_block, file_name, table);
}