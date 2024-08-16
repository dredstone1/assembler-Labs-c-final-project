#include "../header/second_pass.h"
#include <stdio.h>
#include <string.h>


/**
 * @brief Handles duplication of symbols in the symbol table.
 * 
 * It iterates through the symbol table, the amount of labels - 1 times,
 * and for each symbol:
 * 		it searches for a duplicate in the remaining part of the table.
 * 		if a duplicate is found, 
 * 		it will get the two lines where the symbols are defined,
 * 		then it will print an error message
 * 		consisting of the two lines and the positions of the symbols in the lines.
 * 		the it will continue to the next symbol.
 * 
 * @param symbol_table The symbol table to check for duplicates.
 * @param label_amount The number of labels in the symbol table.
 * @param error The error structure to store any errors encountered.
 */
void handle_duplication(symbol_address *symbol_table, int label_amount, error *error);


/**
 * @brief Sets the address of the symbols in the word command list.
 * 
 * It iterates through the word command list,
 * for each word:
 * 		if the symbol is NULL, continue to the next word.
 * 		else, search for the symbol in the symbol table.
 * 		if the symbol is not found,
 * 			get the line where the symbol is not found, and print an error message with the line.
 * 			then continue to the next word.
 * 		if founded symbol is not marked as external,
 * 			insert the address of the symbol into the word, and set the ARE to R(relative).
 * 		else, set the ARE to E(external).
 * 		
 * after the iteration, if the file has been opened, close it.
 * 
 * @param symbol_table The symbol table containing the symbols.
 * @param label_amount The number of labels in the symbol table.
 * @param IC The instruction counter.
 * @param commands The word command list.
 * @param error The error structure.
 * @return int 1 if the function executed successfully, 0 otherwise.
 */
void set_symbol_address(symbol_address *symbol_table, int label_amount, int IC, word_data *commands, error *error);


/**
 * @brief Handles duplication of symbols marked as external in the symbol table and the entries table.
 * 
 * it checks if the amount of entries or the amount of external symbols is 0, if so, return.
 * 
 * It iterates through the symbol table,
 * for each symbol:
 * 		if the symbol is not marked as external, continue to the next symbol.
 * 		else, search for the symbol in the entire entries table.
 * 		if the symbol is found in the entries table:
 * 			get the two lines where the symbols are defined,
 * 			then print an error message consisting of the two lines,
 * 			in the correct order of the lines and the positions of the symbols in the lines.
 * 
 * @param entries The entries to check for duplicates.
 * @param symbol_table The symbol table containing the symbols.
 * @param entry_amount The number of entries.
 * @param extern_amount The number of external symbols.
 * @param error The error structure to store any errors encountered.
 * @param label_amount The number of labels in the symbol table.
 */
void handle_external_n_entry_duplication(symbol_address *entries, symbol_address *symbol_table, int entry_amount,
										 int extern_amount, error *error, int label_amount);


void second_pass(word_data *commands, symbol_address *entries, symbol_address *symbol_table, error *error, int IC,
				 int label_amount, int entry_amount, int extern_amount) {
	/* set the error importance to NO_ERROR */
	error->importance = NO_ERROR;

	/* check for duplication of symbols marked as external in the symbol table
	 * and symbols in the entries table */
	handle_external_n_entry_duplication(entries, symbol_table, entry_amount, extern_amount, error, label_amount);

	/* check for duplication of symbols in the symbol table */
	handle_duplication(symbol_table, label_amount, error);

	/* if there isn't critical or cancellation error,
	 * set the address of the symbols in the word data list
	 * and if a symbol isn't found, print an error message
	 * if there is a system error, print an error message */
	if (error->importance != CANCELLATION && error->importance != CRITICAL) {
		set_symbol_address(symbol_table, label_amount, IC, commands, error);
	}
}


void handle_duplication(symbol_address *symbol_table, int label_amount, error *error) {
	/* initialize two lines to store the lines of the symbols in case of duplication */
	char line[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];

	/* initialize two integers to store the current symbol(current index),
	 * and the found index to store the result of the search in the symbol table */
	int current_symbol, found;

	/* iterate over the symbols in the symbol table
	 * in each iteration, it searches for the current symbol name in the rest of the symbol table,
	 * if its -1, continue to the next symbol, otherwise, print an error message */
	for (current_symbol = 0; current_symbol < label_amount - 1; current_symbol++) {
		found = search_symbol_by_name(symbol_table[current_symbol].symbol_name, (symbol_table + current_symbol + 1),
									  label_amount - current_symbol - 1);
		if (found != -1) {
			/* add the (current symbol index + 1) to the found index to get the correct index in the symbol table */
			found += current_symbol + 1;

			/* get the two lines where the symbols are defined */
			get_line_from_file(line2, symbol_table[current_symbol].line_number, error->file_name, error);
			get_line_from_file(line, symbol_table[found].line_number, error->file_name, error);

			/* print the error message, consisting of the two lines and the positions of the symbols in the lines */
			print_two_line_error(DUPLICATE_SYMBOL_MESSAGE, SYMBOL_ALREADY_EXISTS_DESCRIPTION, line2, line,
								 symbol_table[current_symbol].line_number, symbol_table[found].line_number,
								 strstr(line2, symbol_table[current_symbol].symbol_name) - line2,
								 strstr(line2, symbol_table[current_symbol].symbol_name) - line2 +
								 strlen(symbol_table[current_symbol].symbol_name),
								 strstr(line, symbol_table[found].symbol_name) - line,
								 strstr(line, symbol_table[found].symbol_name) - line +
								 strlen(symbol_table[found].symbol_name), error);
		}
	}
}


void set_symbol_address(symbol_address *symbol_table, int label_amount, int IC, word_data *commands, error *error) {
	/* initialize a line to the line where the symbol is not found */
	char line[MAX_LINE_LENGTH];

	/* initialize the current index of the word command list.
	 * and the current line number to store how many lines needed to skip to print the line,
	 * were the symbol is not found.
	 * and the found index to store the result of the search in the symbol table */
	int word_number, found, current_line_number = 0;

	/* initialize a file pointer to store the file pointer of the file and initialize it to NULL */
	FILE *file = NULL;

	/* iterate over the word command list */
	for (word_number = 0; word_number < IC; word_number++) {
		/* if the symbol is NULL, no symbol is defined in the word, continue to the next word */
		if (commands[word_number].symbol == NULL) {
			continue;
		}

		/* else, symbol is defined in the word, search for the symbol in the entire symbol table */
		found = search_symbol_by_name(commands[word_number].symbol, symbol_table, label_amount);

		/* if the found index is -1, the symbol is not found in the symbol table
		 * print an error message and continue to the next word */
		if (found == -1) {
			/*if the file is not open yet, open it */
			if (file == NULL) {
				file = fopen(error->file_name, "r");

				/* if the file is NULL, print an error message and return 0 */
				if (file == NULL) {
					print_system_error(FILE_NOT_FOUND_MESSAGE, error, CANCELLATION);
					return;
				}
			}

			/* get the line where the symbol is not found */
			go_to_next_line(file, line, commands[word_number].line_number, &current_line_number);

			/* print the error message with the line where the symbol is not found */
			print_general_error(SYMBOL_NOT_FOUND_MESSAGE, SYMBOL_NOT_FOUND_DESCRIPTION, line,
								commands[word_number].line_number,
								strstr(line, commands[word_number].symbol) - line,
								strstr(line, commands[word_number].symbol) - line +
								strlen(commands[word_number].symbol), -1, -1, error);
			/* continue to the next word */
			continue;
		}

		/*if the symbol is not marked as external,
		 * insert the address of the symbol into the word,
		 * and set the ARE to R(relative) */
		if (symbol_table[found].external == 0) {
			insert_operand_into_word(&commands[word_number].word, symbol_table[found].address);
			set_ARE_into_word(&commands[word_number].word, R);
		}
			/* else, the symbol is marked as external,
			 * and set the ARE to E(external) */
		else {
			set_ARE_into_word(&commands[word_number].word, E);
		}

		/* set the external flag of the word to the marked external flag of the symbol */
		commands[word_number].external = symbol_table[found].external;
	}

	/* if the file has been opened, close it */
	if (file != NULL) {
		fclose(file);
	}
}


void handle_external_n_entry_duplication(symbol_address *entries, symbol_address *symbol_table, int entry_amount,
										 int extern_amount, error *error, int label_amount) {
	int i, found;

	/* initialize two lines to store the lines of the symbols in case of duplication */
	char line[MAX_LINE_LENGTH], line2[MAX_LINE_LENGTH];

	/* check if the amount of entries or the amount of external symbols is 0, if so, return */
	if (entry_amount == 0 || extern_amount == 0) {
		return;
	}

	/* iterate over the symbols in the symbol table */
	for (i = 0; i < label_amount; i++) {
		/* if the symbol is not marked as external, continue to the next symbol */
		if (symbol_table[i].external == 0) {
			continue;
		}

		/* else, search for the symbol in the entire entries table */
		found = search_symbol_by_name(symbol_table[i].symbol_name, entries, entry_amount);

		/* if the symbol is found in the entries table */
		if (found != -1) {
			/* get the two lines where the symbols are defined */
			get_line_from_file(line, symbol_table[i].line_number, error->file_name, error);
			get_line_from_file(line2, entries[found].line_number, error->file_name, error);

			/* print the error message, consisting of the two lines and the positions of the symbols in the lines
			 * in the correct order of the lines */
			if (symbol_table[i].line_number > entries[found].line_number) {
				print_two_line_error(DUPLICATE_SYMBOL_MESSAGE, SYMBOL_ALREADY_EXISTS_DESCRIPTION, line2, line,
									 entries[found].line_number, symbol_table[i].line_number,
									 strstr(line2, symbol_table[i].symbol_name) - line2,
									 strstr(line2, symbol_table[i].symbol_name) - line2 +
									 strlen(symbol_table[i].symbol_name),
									 strstr(line, entries[found].symbol_name) - line,
									 strstr(line, entries[found].symbol_name) - line +
									 strlen(entries[found].symbol_name), error);
			} else {
				print_two_line_error(DUPLICATE_SYMBOL_MESSAGE, SYMBOL_ALREADY_EXISTS_DESCRIPTION, line, line2,
									 symbol_table[i].line_number, entries[found].line_number,
									 strstr(line, entries[found].symbol_name) - line,
									 strstr(line, entries[found].symbol_name) - line +
									 strlen(entries[found].symbol_name),
									 strstr(line2, symbol_table[i].symbol_name) - line2,
									 strstr(line2, symbol_table[i].symbol_name) - line2 +
									 strlen(symbol_table[i].symbol_name), error);
			}
		}
	}
}
