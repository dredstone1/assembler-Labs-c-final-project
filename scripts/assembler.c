#include "../header/post_formating.h"
#include "../header/first_pass.h"
#include <stdlib.h>


/**
 * @brief Runs the assembler on the given files.
 * 
 * This function runs the assembler on the given files. It initializes the error struct and the macros array.
 * It then iterates over the files, calling the post_formating function, the first_pass function, and then
 * freeing the macros array and the file name.
 * 
 * @param files_paths An array of strings containing the paths to the files to be assembled.
 * @param number_of_files The number of files to be assembled.
 */

/**
 * @brief Runs the assembler on a set of input files.
 *
 * This function iterates through the provided file paths, performs pre-processing, 
 * executes the first pass of the assembler, and handles memory cleanup.
 * It also manages error handling and program termination, if necessary.
 * based on the severity of encountered errors.
 *
 * @param files_paths An array of strings representing the paths to the input files.
 * @param number_of_files The number of files to process.
 */
void run_assembler(char **files_paths, int number_of_files);


/*
 * Main function that handles the assembler execution.
 * 
 * @param argc The number of command line arguments.
 * @param argv An array of strings containing the command line arguments.
 * 
 * @return 0 if the program executes successfully, otherwise an error code.
 */
int main(int argc, char **argv) {
	/*start the assembler loop*/
	run_assembler(argv, argc);
	return 0;
}

void run_assembler(char **files_paths, int number_of_files) {
	error error;
	int i, number_of_macros;
	char *file_name = NULL;
	macro *macros = NULL;

	/* Iterate through each file path provided */
	for (i = 1; i < number_of_files; ++i) {
		/* Initialize error structure for the current file */
		error.importance = NO_ERROR;
		error.error_single_file_count = 0;
		number_of_macros = 0;
		macros = NULL;

		/* Prepare the file name for processing */
		if (initialize_new_file_name(&file_name, &error, files_paths[i]) == 0){
			return;
		}
		/*create the file name with the correct ending*/
		set_ending_to_file_name(file_name, SOURCE_FILE_ENDING);
		error.file_name = file_name;


		/* Perform pre-processing and macro expansion */
		post_formating(&error, file_name, &macros, &number_of_macros);

		/* If no ERROR occurred during pre-processing, execute the first pass */
		if (error.importance == NO_ERROR) {
			first_pass(file_name, &error, macros, number_of_macros);
		}

		/* Free allocated memory for macros and file name, if necessary */
		handle_free(macros);
		handle_free(file_name);

		/* Print a separator line after processing each file, if any errors occurred */
		if (error.error_single_file_count > 0) {
			print_separator(1);
		}
		
		/* if the error importance is critical, return */
		if (error.importance == CRITICAL){
			return;
		}
	}
}
