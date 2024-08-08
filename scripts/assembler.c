#include "../header/post_formating.h"
#include "../header/first_pass.h"
#include <stdlib.h>

void run_assembler(char **files_paths, int number_of_files);

int main(int argc, char **argv){
	run_assembler(argv, argc);
    return 0;
}

void run_assembler(char **files_paths, int number_of_files){
	error error;
	int i, number_of_macros;
	char *file_name;
	macro *macros = NULL;
	error.error_count = 0;
	
	for (i = 1; i < number_of_files; ++i) {
		error.importance = NO_ERROR;
		error.error_single_file_count = 0;
		number_of_macros = 0;
		macros = NULL;
		initialize_new_file_name(&file_name, &error, files_paths[i]);
		set_ending_to_file_name(file_name, SOURCE_FILE_ENDING);
		error.file_name = file_name;

		if (error.importance == NO_ERROR) {
			post_formating(&error, file_name, &macros, &number_of_macros);
			
			if (error.importance == NO_ERROR)
				first_pass(file_name, &error, macros, number_of_macros);
		}
		if (macros != NULL)
			free(macros);
		free(file_name);
		if (error.error_single_file_count > 0)
			print_separator(1);
	}
}
