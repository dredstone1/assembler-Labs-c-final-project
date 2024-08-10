#include "../header/file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void write_to_file_am(char file_name[], line_text *lines, int number_of_rows) {
	FILE *new_file;
	int current_line;

	set_ending_to_file_name(file_name, SOURCE_FILE_AFTER_POST_ASSEMBLER_ENDING);
	new_file = fopen(file_name, "w");
	if (new_file == NULL) {
		return;
	}

	for (current_line = 0; current_line < number_of_rows; current_line++) {
		fprintf(new_file, "%s", lines[current_line].content);
	}

	fclose(new_file);
	set_ending_to_file_name(file_name, SOURCE_FILE_ENDING);
}


void write_to_file_object(word_data *list1, word_data *list2, char fileName[], int IC, int DC) {
	int current_word_line;
	FILE *file;

	set_ending_to_file_name(fileName, OBJECT_FILE_ENDING);

	file = fopen(fileName, "w");
	if (file == NULL) {
		return;
	}

	fprintf(file, "%5d %d\n", IC, DC);

	for (current_word_line = 0; current_word_line < IC; current_word_line++) {
		fprintf(file, "%04d %05d\n", current_word_line + IC_START_COUNT, int_to_octal(list1[current_word_line].word));
	}

	for (current_word_line = 0; current_word_line < DC; current_word_line++) {
		fprintf(file, "%04d %05d", current_word_line + IC_START_COUNT + IC,
				int_to_octal(list2[current_word_line].word));
		if (current_word_line + 1 < DC) {
			fprintf(file, "\n");
		}
	}

	fclose(file);
	set_ending_to_file_name(fileName, SOURCE_FILE_ENDING);
}


void write_to_file_entry(symbol_address *entries, int entry_amount, symbol_address *labels, int label_amount,
						 char fileName[]) {
	int current_entry, found;
	FILE *file;

	if (entry_amount == 0) {
		return;
	}

	set_ending_to_file_name(fileName, ENTRY_FILE_ENDING);
	file = fopen(fileName, "w");
	if (file == NULL) {
		return;
	}

	for (current_entry = 0; current_entry < entry_amount; current_entry++) {
		found = search_symbol_by_name(entries[current_entry].symbol_name, labels, label_amount);
		if (found == -1) {
			continue;
		}

		fprintf(file, "%s %d", labels[found].symbol_name, labels[found].address);
		if (current_entry + 1 < entry_amount) {
			fprintf(file, "\n");
		}
	}

	fclose(file);
}


void write_to_file_external(word_data *commands, int IC, char fileName[], int extern_amount) {
	int i;
	FILE *file;

	if (extern_amount == 0) {
		return;
	}

	set_ending_to_file_name(fileName, EXTERNAL_FILE_ENDING);
	file = fopen(fileName, "w");
	if (file == NULL) {
		return;
	}

	for (i = 0; i < IC; i++) {
		if (commands[i].external == 1) {
			fprintf(file, "%s %d\n", commands[i].symbol, i + IC_START_COUNT);
		}
	}

	fclose(file);
}


void initialize_new_file_name(char **file_name, error *error, char name[]) {
	*file_name = (char *) use_malloc(sizeof(char) * (strlen(name) + FILE_NAME_ENDING_LENGTH), error);
	if (error->importance != NO_ERROR) {
		return;
	}

	strcpy(*file_name, name);
	(*file_name)[strlen(name)] = '.';
	(*file_name)[strlen(name) + 1] = '\0';
}