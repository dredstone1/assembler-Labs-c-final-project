#include "../header/file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void write_to_file_am(char file_name[], line_text *lines, int number_of_rows, error *error) {
	int current_line;
	FILE *new_file;

	/*set the file name to the new file name ".am"*/
	set_ending_to_file_name(file_name, SOURCE_FILE_AFTER_POST_ASSEMBLER_ENDING);
	/*open the new file for writing*/
	new_file = fopen(file_name, "w");
	/*if the file could not be opened, print an error message and return*/
	if (new_file == NULL) {
		print_system_error(FILE_NOT_FOUND_MESSAGE, error, CANCELLATION);
		return;
	}

	/*write the lines to the new file*/
	for (current_line = 0; current_line < number_of_rows; current_line++) {
		fprintf(new_file, "%s", lines[current_line].content);
	}

	/*close the file*/
	fclose(new_file);
	/*set the file name back to the original name (".as")*/
	set_ending_to_file_name(file_name, SOURCE_FILE_ENDING);
}


void write_to_file_object(word_data *list1, word_data *list2, char fileName[], int IC, int DC, error *error) {
	int current_word_line;
	FILE *file;

	/*set the file name to the new file name ".ob"*/
	set_ending_to_file_name(fileName, OBJECT_FILE_ENDING);

	/*open the new file for writing*/
	file = fopen(fileName, "w");
	/*if the file could not be opened, print an error message and return*/
	if (file == NULL) {
		print_system_error(FILE_NOT_FOUND_MESSAGE, error, CANCELLATION);
		return;
	}

	/*write the IC and DC to the file*/
	fprintf(file, "%*d%*s%d\n", LENGTH_OF_WORD_ADDRESS_TO_BE_PRINTED, IC, SPACE_BETWEEN_SUBJECT_IC_DC_TO_BE_PRINTED, "", DC);

	/*write the commands words to the file*/
	for (current_word_line = 0; current_word_line < IC; current_word_line++) {
		fprintf(file, "%0*d%*s%0*d\n", LENGTH_OF_WORD_ADDRESS_TO_BE_PRINTED, current_word_line + OS_SAVED_MEMORY_CELLS,
				SPACE_BETWEEN_WORD_ADDRESS_AND_CONTENT_TO_BE_PRINTED, "", LENGTH_OF_WORD_CONTENT_TO_BE_PRINTED,
				int_to_octal(list1[current_word_line].word));
	}
	
	/*write the data words to the file*/
	for (current_word_line = 0; current_word_line < DC; current_word_line++) {
		fprintf(file, "%0*d%*s%0*d", LENGTH_OF_WORD_ADDRESS_TO_BE_PRINTED, current_word_line + OS_SAVED_MEMORY_CELLS + IC,
				SPACE_BETWEEN_WORD_ADDRESS_AND_CONTENT_TO_BE_PRINTED, "",
				LENGTH_OF_WORD_CONTENT_TO_BE_PRINTED, int_to_octal(list2[current_word_line].word));
		if (current_word_line + 1 < DC) {
			fprintf(file, "\n");
		}
	}

	/*close the file*/
	fclose(file);
	
	/*set the file name back to the original name (".as")*/
	set_ending_to_file_name(fileName, SOURCE_FILE_ENDING);
}


void write_to_file_entry(symbol_address *entries, int entry_amount, symbol_address *labels, int label_amount,
						 char fileName[], error *error) {
	int current_entry, found, max_symbol_name_length = 0;
	FILE *file;
	
	/*if there are no entries, return*/
	if (entry_amount == 0) {
		return;
	}

	/*set the file name to the new file name ".ent"*/
	set_ending_to_file_name(fileName, ENTRY_FILE_ENDING);
	/*open the new file for writing*/
	file = fopen(fileName, "w");
	/*if the file could not be opened, print an error message and return*/
	if (file == NULL) {
		print_system_error(FILE_NOT_FOUND_MESSAGE, error, CANCELLATION);
		return;
	}
	
	/*find the longest symbol name*/
	for (current_entry = 0; current_entry < entry_amount; current_entry++) {
		if (search_symbol_by_name(entries[current_entry].symbol_name, labels, label_amount) != -1) {
			max_symbol_name_length = max(strlen(entries[current_entry].symbol_name), max_symbol_name_length);
		}
	}
	
	/*write the entries to the file*/
	for (current_entry = 0; current_entry < entry_amount; current_entry++) {
		/*search for the entry in the symbol table*/
		found = search_symbol_by_name(entries[current_entry].symbol_name, labels, label_amount);
		if (found == -1) {
			continue;
		}
		
		/*write the entry to the file*/
		fprintf(file, "%-*s%*s%0*d", max_symbol_name_length, entries[current_entry].symbol_name, SPACE_BETWEEN_SYMBOL_NAME_AND_ADDRESS_OF_ENTRY_TO_BE_PRINTED, "", LENGTH_OF_WORD_ADDRESS_OF_ENTRY_TO_BE_PRINTED, labels[found].address);
		
		/*if there are more entries, write a new line*/
		if (current_entry + 1 < entry_amount) {
			fprintf(file, "\n");
		}
	}

	/*close the file*/
	fclose(file);
}


void write_to_file_external(word_data *commands, int IC, char fileName[], int extern_amount, error *error) {
	int i, amount_of_extern = 0, longest_symbol_name = 0;
	FILE *file;
	
	/*if there are no external symbols, return*/
	if (extern_amount == 0) {
		return;
	}
	
	/*set the file name to the new file name ".ext"*/
	set_ending_to_file_name(fileName, EXTERNAL_FILE_ENDING);
	/*open the new file for writing*/
	file = fopen(fileName, "w");
	/*if the file could not be opened, print an error message and return*/
	if (file == NULL) {
		print_system_error(FILE_NOT_FOUND_MESSAGE, error, CANCELLATION);
		return;
	}
	
	/*find the longest symbol name*/
	for (i = 0; i < IC; i++) {
		/*if the command is external, check if the symbol name is longer than the current longest symbol name
		 * if it is, set the longest symbol name to the length of the current symbol name*/
		if (commands[i].external == 1) {
			longest_symbol_name = max(strlen(commands[i].symbol), longest_symbol_name);
			
			/*increment the amount of external symbols*/
			extern_amount++;
		}
	}
	
	/*write the external symbols to the file*/
	for (i = 0; i < IC; i++) {
		/*if the command is external, write the symbol name and the address to the file*/
		if (commands[i].external == 1) {
			/*write new line if it is not the first external symbol to be written*/
			if (amount_of_extern > 0) {
				fprintf(file, "\n");
			}
			
			/*write the external symbol to the file*/
			fprintf(file, "%-*s%*s%0*d", longest_symbol_name, commands[i].symbol, SPACE_BETWEEN_SYMBOL_NAME_AND_ADDRESS_TO_BE_PRINTED, "", LENGTH_OF_WORD_ADDRESS_OF_EXTERNAL_TO_BE_PRINTED, i + OS_SAVED_MEMORY_CELLS);
			
			/*increment the amount of external symbols written*/
			amount_of_extern++;
		}
		
		/*if the amount of external symbols written is equal to the amount of external symbols, break*/
		if (amount_of_extern == extern_amount) {
			break;
		}
	}
	
	/*close the file*/
	fclose(file);
}


int initialize_new_file_name(char **file_name, error *error, char name[]) {
	/*allocate memory for the new file name*/
	*file_name = (char *) use_malloc(sizeof(char) * (strlen(name) + FILE_NAME_ENDING_LENGTH), error);
	/*if the memory allocation failed, return 0*/
	if (*file_name == NULL) {
		return 0;
	}

	/*copy the base name to the new file name*/
	strcpy(*file_name, name);
	
	/*set the ending of the file name to .xxx*/
	(*file_name)[strlen(name)] = '.';
	(*file_name)[strlen(name) + 1] = '\0';
	return 1;
}