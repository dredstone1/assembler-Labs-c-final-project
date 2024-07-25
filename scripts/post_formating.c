#include "../header/file.h"
#include "../header/macro.h"
#include <stdio.h>
#include <string.h>

void post_formating(error_array *error, char file_name[]){
    macro *macros;
	line_node *first_line = NULL;
	FILE *file;
	int macro_exist = 0, reading_macro = 0, number_of_rows = 0, number_of_macros = 0;
	
	set_ending_to_file_name(file_name, "as");
	file = fopen(file_name, "r");
	if (file == NULL) {
		add_error(error, FILE_NOT_FOUND, 0, 0, 0, CRITICAL, file_name, 0);
		return;
	}
	
	char line[MAX_LINE_LENGTH];
	char *workable_line, *start_workable_line;

	while (fgets(line, MAX_LINE_LENGTH, file) != NULL && error->importance != WARNING) {
		workable_line = start_workable_line;
		
		workable_line = strtok(workable_line, " \t");
		macro_exist = is_line_macro(workable_line);

		
		if (macro_exist == 1) {
			if (reading_macro == 0) {
				workable_line = strtok(workable_line, " \t");
				add_macro(workable_line, &macros, &number_of_macros, error);
			}	else {
				workable_line[] += strlen(workable_line) + 1;
				if (macros[number_of_macros].number_of_macro_lines == 0){
					macros[number_of_macros].first_macro_lines = create_line_node(NULL, );
					macros[number_of_macros].last_macro_lines = create_line_node(NULL, workable_line);
				else
					macros[number_of_macros].macro_lines[macros[number_of_macros].number_of_macro_lines] = create_line_node(NULL, workable_line);
					
			}
		}
		
		if (is_line_macro(workable_line)==1)
			macro_exist = 1;
			workable_line = strtok(workable_line, " \t");
			add_macro(workable_line, read_macro_lines(node), macros, error);
			number_of_rows -= macros->macro[macros->number_of_macros - 1].number_of_macro_lines + 2;
			offset_line_node_by(*node, -(macros->macro[macros->number_of_macros - 1].number_of_macro_lines+2));
		}
		if (macros->number_of_macros>0) {
			temp = get_macro_from_name(macros, *node);
			if (temp != NULL) {
				number_of_rows += temp->number_of_macro_lines;
				replace_line_to_macro(*temp, node);
			}
		}
		node = &(*node)->next;
	}
	
/*
    handle_macros(&file->first_line, &file->number_of_rows, &macros, error);
*/
    write_to_file(new_file);
    free_macros(&macros);
}

