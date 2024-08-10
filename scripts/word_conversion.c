#include "../header/word_conversion.h"
#include <stdlib.h>

int get_amount_of_words_from_command(command_data command) {
	/*Get the amount_of_words of variables from the opcode, then add 1 the first word*/
	int amount_of_words = amount_of_variables_from_opcode(command.opcode) + 1;

	/*If both the source and destination are registers, and the amount_of_words of is 3, decrease the amount_of_words by 1,
	 because the two registers can be in the same word*/
	if (command.source.type > 1 && command.destination.type > 1 && amount_of_words == 3) {
		amount_of_words--;
	}

	return amount_of_words;
}


void reset_word(word_data *words, int line_number) {
	/*Reset the word_data structure to its initial state, and set the line number to the given line_number*/
	words->word = 0;
	words->symbol = NULL;
	words->line_number = line_number;
	words->external = 0;
}


int add_data_to_words(word_data **list, directive_data directive, int line_number, int *DC, error *error) {
	int i;

	/*Allocate memory for the new data, then check if the allocation was successful*/
	*list = (word_data *) use_realloc(*list, sizeof(word_data) * (directive.size + (*DC)), error);
	if (*list == NULL) {
		return 0;
	}

	/*For each number in the directive, reset the word_data structure at the current DC, then insert the number into the corresponding word*/
	for (i = 0; i < directive.size; ++i) {
		reset_word(&(*list)[i + *DC], line_number);
		insert_value_into_word(&(*list)[i + *DC].word, directive.numbers[i]);
	}

	/*Increment the data counter by the size of the directive*/
	*DC += directive.size;
	return 1;
}


int add_command_to_words(word_data **list, command_data command, error *error, int *IC, int line_number) {
	/*Calculate the number of operands (source and/or destination) for the command*/
	int amount_of_operands = amount_of_variables_from_opcode(command.opcode);

	*list = (word_data *) use_realloc(*list, sizeof(word_data) * (get_amount_of_words_from_command(command) + *IC),
									  error);
	if (error->importance == CRITICAL) {
		return 0;
	}

	/*Reset the current word and potential following words based on the amount_of_words needed for the command*/
	reset_word(*list + *IC, line_number);
	if (get_amount_of_words_from_command(command) > 1) {
		reset_word(*list + *IC + 1, line_number);
	}
	if (get_amount_of_words_from_command(command) > 2) {
		reset_word(*list + *IC + 2, line_number);
	}

	/*Set the opcode in the first word*/
	set_opcode_into_word(&((*list)[*IC].word), command.opcode);

	/*Insert the A into the first word*/
	set_ARE_into_word(&((*list)[*IC].word), A);

	/*check if the amount of operands is 2, if so*/
	if (amount_of_operands == 2) {
		/*If there are two operands, insert the source and destination types into the first word*/
		insert_operand_type_into_word(&((*list)[*IC].word), DESTINATION, command.destination.type);
		insert_operand_type_into_word(&((*list)[*IC].word), SOURCE, command.source.type);

		/*If the source is immediate, insert the value into the second word*/
		if (command.source.type == IMMEDIATE) {
			insert_operand_into_word(&((*list)[*IC + 1].word), command.source.value);
			set_ARE_into_word(&((*list)[*IC + 1].word), A);
		}
			/*If the source is direct, insert the symbol into the second word, for later address resolution*/
		else if (command.source.type == DIRECT) {
			(*list)[*IC + 1].symbol = duplicate_string(command.source.var, error);
		}
			/*If the source is register indirect or register direct, insert the value into the second word
			* if the destination is also a register, insert the value of the destination into the second word, as well, without overwriting the source value*/
		else if (command.source.type == REGISTER_INDIRECT || command.source.type == REGISTER_DIRECT) {
			insert_operand_into_word(&((*list)[*IC + 1].word), command.source.value << operand_bit_shift);

			/*If the destination is register indirect or register direct, insert the value into the second word*/
			if (command.destination.type > 1) {
				insert_operand_into_word(&((*list)[*IC + 1].word), command.destination.value);
			}

			set_ARE_into_word(&((*list)[*IC + 1].word), A);
		}

		/*If both the source and destination are registers, return, as they were already encoded in the second word*/
		if ((command.source.type == REGISTER_INDIRECT || command.source.type == REGISTER_DIRECT) &&
			(command.destination.type == REGISTER_INDIRECT || command.destination.type == REGISTER_DIRECT)) {
			return 1;
		}

		/*If the destination is immediate, insert the value into the third word*/
		if (command.destination.type == IMMEDIATE) {
			insert_operand_into_word(&((*list)[*IC + 2].word), command.destination.value);
			set_ARE_into_word(&((*list)[*IC + 2].word), A);
		}
			/*If the destination is direct, insert the symbol into the third word, for later address resolution*/
		else if (command.destination.type == DIRECT) {
			(*list)[*IC + 2].symbol = duplicate_string(command.destination.var, error);
		}
			/*If the destination is register indirect or register direct, insert the value into the third word*/
		else if (command.destination.type == REGISTER_INDIRECT || command.destination.type == REGISTER_DIRECT) {
			insert_operand_into_word(&((*list)[*IC + 2].word), command.destination.value);
			set_ARE_into_word(&((*list)[*IC + 2].word), A);
		}
		/*If there is only one operand*/
	} else if (amount_of_operands == 1) {
		/*If there is only one operand, insert the destination type into the first word*/
		insert_operand_type_into_word(&((*list)[*IC].word), DESTINATION, command.destination.type);

		/*If the destination is immediate, insert the value into the second word*/
		if (command.destination.type == IMMEDIATE) {
			insert_operand_into_word(&((*list)[*IC + 1].word), command.destination.value);
			set_ARE_into_word(&((*list)[*IC + 1].word), A);
		}
			/*If the destination is direct, insert the symbol into the second word, for later address resolution*/
		else if (command.destination.type == DIRECT) {
			(*list)[*IC + 1].symbol = duplicate_string(command.destination.var, error);
		}
			/*If the destination is register indirect or register direct, insert the value into the second word*/
		else if (command.destination.type == REGISTER_INDIRECT || command.destination.type == REGISTER_DIRECT) {
			insert_operand_into_word(&((*list)[*IC + 1].word), command.destination.value);
			set_ARE_into_word(&((*list)[*IC + 1].word), A);
		}
	}

	return 1;
}

void set_opcode_into_word(word *word, opcode opcode) {
	/*Shift the opcode left by opcode_bit_shift, then call insert_value_into_word to insert the new value into the word*/
	*word |= opcode << opcode_bit_shift;
}

void insert_operand_type_into_word(word *word, operand operand, variable_type type) {
	/*Shift '1' by the type, then shift the result by operand_bit_shift + (operand_bit_size * operand),
	then call insert_value_into_word to insert the new value into the word*/
	insert_value_into_word(word, (1 << type) << (operand_bit_shift + operand_bit_size * operand));
}

void set_ARE_into_word(word *word, ARE ARE) {
	/*Shift '1' by the ARE, then call insert_value_into_word to insert the new value into the word, without overwriting the other bits*/
	insert_value_into_word(word, 1 << ARE);
}

void insert_operand_into_word(word *word, int value) {
	/*Shift the value left by operand_bit_shift, then call insert_value_into_word to insert the new value into the word, without overwriting the other bits*/
	insert_value_into_word(word, value << operand_bit_shift);
}

void insert_value_into_word(word *word, short value) {
	/*Apply the mask to the value to ensure only relevant bits are used,
	then insert the value into the word, without overwriting the other bits*/
	*word |= value & SHORT_MAX_BIT_SHIFT;
}

void free_word_data_list(word_data *words, int length) {
	int i;

	/*Free the symbol strings in each word_data structure(if they been allocated), then free the word_data array.*/
	for (i = 0; i < length; ++i) {
		/*If the symbol is not NULL, free the memory allocated for the symbol*/
		handle_free(words[i].symbol);
	}

	/*Free the word_data array, if it has been allocated*/
	handle_free(words);
}
