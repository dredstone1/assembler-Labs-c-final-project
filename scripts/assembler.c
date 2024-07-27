#include "../header/post_formating.h"
#include "../header/first_pass.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void run_assembler(char **files_paths, int number_of_files);

int main(int argc, char **argv){
    clock_t tic, toc, total_count = 10;
	float average_time = 0.0;
	int i;
	for (i = 0; i < total_count; i++) {
		tic = clock();
		run_assembler(argv, argc);
		toc = clock();
		average_time +=(double) (toc - tic) / CLOCKS_PER_SEC;
	}
    printf("average time: %f seconds", ((average_time) / total_count));
	getchar();
    return 0;
}

void run_assembler(char **files_paths, int number_of_files){
	error_array error;
	int i, error_count = 0;
	char *file_name;
	
	for (i = 1; i < number_of_files; ++i) {
		initialize_error(&error);
		error.importance = NO_ERROR;
		initialize_new_file_name(&file_name, &error, files_paths[i]);
		set_ending_to_file_name(file_name, "as");

		if (error.importance == NO_ERROR) {
			post_formating(&error, file_name);
			first_pass(file_name, &error);
		}
		
		handel_error(error, file_name);
		error_count += error.size-1;
		free(error.errors);
		free(file_name);
	}
	printf("total errors: %d\n", error_count);
}
