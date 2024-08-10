#include "../header/second_pass.h"
#include <stdio.h>
#include <string.h>

void handle_duplication(symbol_address *symbol_table, int label_amount, error *error);

int set_symbol_address(symbol_address *symbol_table, int label_amount, int IC, word_data *commands, error *error);

void handle_external_n_entry_duplication(symbol_address *entries, symbol_address *symbol_table, int entry_amount,
										 int extern_amount, error *error, int label_amount);

void second_pass(word_data *commands, word_data *directive, symbol_address *entries, symbol_address *symbol_table,
				 error *error, int IC, int DC, int label_amount, int entry_amount, int extern_amount) {
	handle_external_n_entry_duplication(entries, symbol_table, entry_amount, extern_amount, error, label_amount);
	handle_duplication(symbol_table, label_amount, error);

	if (error->importance == NO_ERROR) {
		set_symbol_address(symbol_table, label_amount, IC, commands, error);
	}
}


void handle_duplication(symbol_address *symbol_table, int label_amount, error *error) {
	char line[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];
	int current_symbol, found;

	for (current_symbol = 0; current_symbol < label_amount - 1; current_symbol++) {
		found = search_symbol_by_name(symbol_table[current_symbol].symbol_name, &symbol_table[current_symbol + 1],
									  label_amount - current_symbol - 1);
		if (found != -1) {
			found += current_symbol + 1;
			get_line_from_file(line2, symbol_table[current_symbol].line_number, error->file_name, error);
			get_line_from_file(line, symbol_table[found].line_number, error->file_name, error);

			print_two_line_error(DUPLICATE_SYMBOL_MESSAGE, SYMBOL_ALREADY_EXISTS_DESCRIPTION, line2, line, symbol_table[current_symbol].line_number, symbol_table[found].line_number, strstr(line2, symbol_table[current_symbol].symbol_name) - line2, strstr(line2, symbol_table[current_symbol].symbol_name) - line2 + strlen(symbol_table[current_symbol].symbol_name), strstr(line, symbol_table[found].symbol_name) - line, strstr(line, symbol_table[found].symbol_name) - line + strlen(symbol_table[found].symbol_name), error);
		}
	}
}

int set_symbol_address(symbol_address *symbol_table, int label_amount, int IC, word_data *commands, error *error) {
	char line[MAX_LINE_LENGTH];
	int word_number, found, current_line_number = 0;
	FILE *file = NULL;

	for (word_number = 0; word_number < IC; word_number++) {
		if (commands[word_number].symbol == NULL) {
			continue;
		}

		found = search_symbol_by_name(commands[word_number].symbol, symbol_table, label_amount);
		if (found == -1) {
			if (file == NULL) {
				file = fopen(error->file_name, "r");

				if (file == NULL) {
					print_system_error(FILE_NOT_FOUND_MESSAGE);
					error->importance = CRITICAL;
					return 0;
				}
			}

			go_to_next_line(file, line, commands[word_number].line_number, &current_line_number);
			print_general_error(SYMBOL_NOT_FOUND_MESSAGE, SYMBOL_NOT_FOUND_DESCRIPTION, line,
								commands[word_number].line_number,
								strstr(line, commands[word_number].symbol) - line,
								strstr(line, commands[word_number].symbol) - line +
								strlen(commands[word_number].symbol), -1, -1, error);
			error->importance = WARNING;
			continue;
		}

		if (symbol_table[found].external == 0) {
			insert_operand_into_word(&commands[word_number].word, symbol_table[found].address);
			set_ARE_into_word(&commands[word_number].word, R);
		} else {
			set_ARE_into_word(&commands[word_number].word, E);
		}

		commands[word_number].external = symbol_table[found].external;
	}

	if (file != NULL) {
		fclose(file);
	}

	return 1;
}

void handle_external_n_entry_duplication(symbol_address *entries, symbol_address *symbol_table, int entry_amount,
										 int extern_amount, error *error, int label_amount) {
	int i, found;
	char line[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];

	if (entry_amount == 0 || extern_amount == 0) {
		return;
	}

	for (i = 0; i < label_amount; i++) {
		if (symbol_table[i].external == 0) {
			continue;
		}

		found = search_symbol_by_name(symbol_table[i].symbol_name, entries, entry_amount);
		if (found != -1) {
			get_line_from_file(line, symbol_table[i].line_number, error->file_name, error);
			get_line_from_file(line2, entries[found].line_number, error->file_name, error);
			if (symbol_table[i].line_number > entries[found].line_number) {
				print_two_line_error(DUPLICATE_SYMBOL_MESSAGE, SYMBOL_ALREADY_EXISTS_DESCRIPTION, line2, line,
									 entries[found].line_number, symbol_table[i].line_number,
									 strstr(line2, symbol_table[i].symbol_name) - line2 + 1,
									 strstr(line2, symbol_table[i].symbol_name) - line2 +
									 strlen(symbol_table[i].symbol_name) + 1,
									 strstr(line, entries[found].symbol_name) - line + 1,
									 strstr(line, entries[found].symbol_name) - line +
									 strlen(entries[found].symbol_name) + 1, error);
			} else {
				print_two_line_error(DUPLICATE_SYMBOL_MESSAGE, SYMBOL_ALREADY_EXISTS_DESCRIPTION, line, line2,
									 symbol_table[i].line_number, entries[found].line_number,
									 strstr(line, entries[found].symbol_name) - line + 1,
									 strstr(line, entries[found].symbol_name) - line +
									 strlen(entries[found].symbol_name) + 1,
									 strstr(line2, symbol_table[i].symbol_name) - line2 + 1,
									 strstr(line2, symbol_table[i].symbol_name) - line2 +
									 strlen(symbol_table[i].symbol_name) + 1, error);
			}

			error->importance = WARNING;
		}
	}
}
