#include "../header/file.h"
#include "../header/post_formating.h"
#include <stdio.h>
#include <string.h>


/**
 * @brief Check if a line is a macro definition line
 * 
 * @param line The line to check
 * @return int 1 if the line is a macro line, 0 if its not
 */
int is_line_macro(const char line[]);


/**
 * @brief add a macro to a macro array
 * 
 * @param macro_name The name of the macro
 * @param macros The macros struct to add the macro to
 * @param error The error array to add errors to
*/
void add_macro(char macro_name[], macro **macros, int *number_of_macros, error_array *error);


/**
 * @brief Check if a line is an end macro line
 * 
 * @param line The line to check
 * @return int 1 if the line is an end macro line, 0 if its not
 */
int is_ending_macro(const char line[]);


/**
 * @brief Search for a macro by its name
 * 
 * @param macro_name The name of the macro to search for
 * @param macros The array of macros to search in
 * @param number_of_macros The number of macros in the array
 * @return int The index of the macro in the array, -1 if the macro was not found
 */
int search_macro_by_name(const char macro_name[], macro *macros, int number_of_macros);


/**
 * @brief Free the memory allocated for the macros
 * 
 * @param macros The macros array to free
 * @param number_of_macros The number of macros in the array
 */
void free_macros(macro *macros, int number_of_macros);



void post_formating(error_array *error, char file_name[]) {
	macro *macros = NULL;
	line_text *new_file = NULL;
	FILE *file;
	
	int macro_exist = 0, number_of_rows = 0, number_of_macros = 0, search_result;
	char line[MAX_LINE_LENGTH], start_workable_line[MAX_LINE_LENGTH], *workable_line;

	set_ending_to_file_name(file_name, "as");
	file = fopen(file_name, "r");
	if (file == NULL) {
		add_error(error, FILE_NOT_FOUND, 0, 0, 0, CRITICAL, file_name, 0, 0);
		return;
	}

	while (fgets(line, MAX_LINE_LENGTH, file) != NULL && error->importance != WARNING) {
		workable_line = start_workable_line;
		strcpy(workable_line, line);
		
		workable_line = strtok(workable_line, " \t\n");
		
		if (workable_line == NULL)
			workable_line = start_workable_line;
		else {
			if (is_line_macro(workable_line) == 1) {
				macro_exist = 1;
				workable_line = strtok(NULL, " \t\n");
				add_macro(workable_line, &macros, &number_of_macros, error);
				continue;
			}
			if (is_ending_macro(workable_line) == 1) {
				macro_exist = 0;
				continue;
			}
			
			search_result = search_macro_by_name(workable_line, macros, number_of_macros);
			if (search_result != -1) {
				new_file = use_realloc(new_file, (number_of_rows + macros[search_result].number_of_macro_lines) * sizeof(line_text), error);
				number_of_rows += macros[search_result].number_of_macro_lines;
				
				for (int i = 0; i < macros[search_result].number_of_macro_lines; i++)
					new_file[number_of_rows - macros[search_result].number_of_macro_lines + i] = macros[search_result].lines[i];
				
				continue;
			}
		}
		
		if (macro_exist == 1) {
			macros[number_of_macros-1].number_of_macro_lines++;
			macros[number_of_macros-1].lines = use_realloc(macros[number_of_macros-1].lines, macros[number_of_macros-1].number_of_macro_lines * sizeof(line_text), error);
			macros[number_of_macros-1].lines[macros[number_of_macros-1].number_of_macro_lines - 1].content = strdup(line);
			macros[number_of_macros-1].lines[macros[number_of_macros-1].number_of_macro_lines - 1].allocated = 0;
		}else{
			number_of_rows++;
			new_file = use_realloc(new_file, (number_of_rows) * sizeof(line_text), error);
			new_file[number_of_rows - 1].content = strdup(line);
			new_file[number_of_rows - 1].allocated = 1;
		}
	}


	fclose(file);
	write_to_file(file_name, new_file, number_of_rows);
	free_lines(new_file, number_of_rows);
	free_macros(macros, number_of_macros);
}

int search_macro_by_name(const char macro_name[], macro *macros, int number_of_macros){
	for (int i = 0; i < number_of_macros; i++) {
		if (strcmp(macro_name, macros[i].macro_name)==0)
			return i;
	}
	return -1;
}

void free_macros(macro *macros, int number_of_macros){
	int i;
	for (i = 0; i < number_of_macros; i++)
		free_lines(macros[i].lines, macros[i].number_of_macro_lines);

	free(macros);
}

void add_macro(char macro_name[], macro **macros, int *number_of_macros, error_array *error){
	(*number_of_macros)++;
	*macros = (macro*)use_realloc(*macros, (*number_of_macros) * sizeof(macro), error);
	if (*macros == NULL)
		return;

	strcpy(macros[(*number_of_macros) - 1]->macro_name, macro_name);
	macros[(*number_of_macros) - 1]->number_of_macro_lines = 0;
	macros[(*number_of_macros) - 1]->lines = NULL;
}

int is_ending_macro(const char line[]){
	return strcmp(line, END_MACRO)==0;
}

int is_line_macro(const char line[]){
	return strcmp(line, MACRO)==0;
}