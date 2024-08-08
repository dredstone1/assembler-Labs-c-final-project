#include "../header/word_conversion.h"
#include <stdlib.h>

int get_amount_of_words_from_command(command_data command){
	int amount = amount_of_variables_from_opcode(command.opcode) + 1;
	
	if (command.source.type>1 && command.destination.type>1 && amount == 3)
		amount--;
	
	return amount;
}


int add_instruction_to_words(word_data *list, instruction_data instruction, int line_number, int *DC) {
	int i;
	
	for (i = 0; i < instruction.size; ++i) {
		list[i + (*DC)].word = 0;
		insert_value_into_word(&list[i + (*DC)].word, instruction.numbers[i]);
		list[i + (*DC)].line_number = line_number;
		list[i + (*DC)].symbol = NULL;
	}
	
	*DC += instruction.size;
	
	return 1;
}

void reset_word(word_data *words, int line_number) {
	words->word = 0;
	words->symbol = NULL;
	words->line_number = line_number;
	words->external = 0;
}

int add_command_to_words(word_data *list, command_data command, error *error, int *IC, int line_number) {
	int amount_of_operands = amount_of_variables_from_opcode(command.opcode);

	reset_word(list + (*IC), line_number);
	
	if (get_amount_of_words_from_command(command)>1)
		reset_word(list + (*IC) + 1, line_number);
	if (get_amount_of_words_from_command(command)>2)
		reset_word(list + (*IC) + 2, line_number);
	
	set_opcode_into_word(&(list[(*IC)].word), command.opcode);
	if (amount_of_operands == 2) {
		insert_operand_type_into_word(&(list[(*IC)].word), DESTINATION, command.destination.type);
		insert_operand_type_into_word(&(list[(*IC)].word), SOURCE, command.source.type);
	} else if (amount_of_operands == 1)
		insert_operand_type_into_word(&(list[(*IC)].word), DESTINATION, command.destination.type);

	set_ARE_into_word(&(list[(*IC)].word), A);

	if (amount_of_operands == 2) {
		if (command.source.type == IMMEDIATE) {
			insert_operand_into_word(&(list[(*IC) + 1].word), command.source.value);
			set_ARE_into_word(&(list[(*IC) + 1].word), A);
		} else if (command.source.type == DIRECT)
			list[(*IC) + 1].symbol = duplicate_string(command.source.var, error);
		else if (command.source.type == REGISTER_INDIRECT || command.source.type == REGISTER_DIRECT) {
			insert_operand_into_word(&(list[(*IC) + 1].word), command.source.value << operand_bit_shift);

			if (command.destination.type > 1)
				insert_operand_into_word(&(list[(*IC) + 1].word), command.destination.value);
			
			set_ARE_into_word(&(list[(*IC) + 1].word), A);
		}
		
		if ((command.source.type == REGISTER_INDIRECT || command.source.type == REGISTER_DIRECT) && (command.destination.type == REGISTER_INDIRECT || command.destination.type == REGISTER_DIRECT))
			return 0;
		if (command.destination.type == IMMEDIATE) {
			insert_operand_into_word(&(list[(*IC) + 2].word), command.destination.value);
			set_ARE_into_word(&(list[(*IC) + 2].word), A);
		} else if (command.destination.type == DIRECT)
			list[(*IC) + 2].symbol = duplicate_string(command.destination.var, error);
		else if (command.destination.type == REGISTER_INDIRECT || command.destination.type == REGISTER_DIRECT) {
			insert_operand_into_word(&(list[(*IC) + 2].word), command.destination.value);
			set_ARE_into_word(&(list[(*IC) + 2].word), A);
		}
	} else if (amount_of_operands == 1) {
		if (command.destination.type == IMMEDIATE) {
			insert_operand_into_word(&(list[(*IC) + 1].word), command.destination.value);
			set_ARE_into_word(&(list[(*IC) + 1].word), A);
		} else if (command.destination.type == DIRECT)
			list[(*IC) + 1].symbol = duplicate_string(command.destination.var, error);
		else if (command.destination.type == REGISTER_INDIRECT || command.destination.type == REGISTER_DIRECT) {
			insert_operand_into_word(&(list[(*IC) + 1].word), command.destination.value);
			set_ARE_into_word(&(list[(*IC) + 1].word), A);
		}
	}
	return 1;
}

void set_opcode_into_word(word *word, opcode op_code){
	*word |= op_code<<opcode_bit_shift;
}

void insert_operand_type_into_word(word *word, operand op, variable_type type){
	insert_value_into_word(word, (1<<type)<<(operand_bit_shift+operand_bit_size*op));
}

void set_ARE_into_word(word *word, ARE are){
	insert_value_into_word(word, 1<<are);
}

void insert_operand_into_word(word *word, int value){
	insert_value_into_word(word, value<<operand_bit_shift);
}

void insert_value_into_word(word *word, short value){
	(*word) |= value & 0x7FFF;
}

void free_word_data_list(word_data *words, int length){
	int i;
	for (i = 0; i < length; ++i)
		if (words[i].symbol != NULL)
			free(words[i].symbol);

	if (length > 0)
		free(words);
}
