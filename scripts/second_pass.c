#include "../header/second_pass.h"
#include <stdio.h>
#include <string.h>


void second_pass(word_data *commands, word_data *instructions, symbol_address *entries, symbol_address *symbol_table, symbol_address **externals, error_array *error, int IC, int DC, int label_amount, int entry_amount, int extern_amount) {
	for (int i = 0; i < label_amount; ++i) {
		printf("test: %s\n", symbol_table[i].symbol_name);
		if (strcmp(symbol_table[i].symbol_name, "L3") == 0)
			printf("found\n");
		insert_symbol_address_into_words(commands, IC, symbol_table[i]);
	}
}
