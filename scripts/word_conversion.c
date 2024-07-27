#include "../header/word_conversion.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int get_amount_of_words_from_command(command_data command){
	int amount = 1;
	amount += amount_of_variables_from_opcode(command.opcode);
	if (command.source.type>1 && command.destination.type>1)
		amount--;
	return amount;
}


int add_instruction_to_words(word_data *list, instruction_data instruction, int line_number, error_array *error, int *DC) {
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


int add_command_to_words(word_data *list, command_data command, int line_number, error_array *error, int *IC) {
	int amount_of_operands = amount_of_variables_from_opcode(command.opcode);
	list[*IC].word = 0;
	list[*IC].symbol = NULL;
	if (get_amount_of_words_from_command(command)>1) {
		list[*IC + 1].word = 0;
		list[*IC + 1].symbol = NULL;
	}if (get_amount_of_words_from_command(command)>2) {
		list[*IC + 2].word = 0;
		list[*IC + 2].symbol = NULL;
	}
	
	set_opcode_into_word(&(list[(*IC)].word), command.opcode);
	if (amount_of_operands == 2) {
		insert_operand_type_into_word(&(list[(*IC)].word), DESTINATION, command.destination.type);
		insert_operand_type_into_word(&(list[(*IC)].word), SOURCE, command.source.type);
	} else if (amount_of_operands == 1)
		insert_operand_type_into_word(&(list[(*IC)].word), DESTINATION, command.source.type);

	set_ARE_into_word(&(list[(*IC)].word), A);

	if (amount_of_operands == 2) {
		if (command.source.type == IMMEDIATE) {
			insert_operand_into_word(&(list[(*IC) + 1].word), command.source.value);
			set_ARE_into_word(&(list[(*IC) + 1].word), A);
		} else if (command.source.type == DIRECT) {
			list[(*IC) + 1].symbol = command.source.var;
		} else if (command.source.type == REGISTER_INDIRECT || command.source.type == REGISTER_DIRECT) {
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
		} else if (command.destination.type == DIRECT) {
			list[(*IC) + 2].symbol = command.destination.var;
		} else if (command.destination.type == REGISTER_INDIRECT || command.destination.type == REGISTER_DIRECT) {
			insert_operand_into_word(&(list[(*IC) + 2].word), command.destination.value);
			set_ARE_into_word(&(list[(*IC) + 2].word), A);
		}
	} else if (amount_of_operands == 1) {
		if (command.source.type == IMMEDIATE) {
			insert_operand_into_word(&(list[(*IC) + 1].word), command.source.value);
			set_ARE_into_word(&(list[(*IC) + 1].word), A);
		} else if (command.source.type == DIRECT) {
			list[(*IC) + 1].symbol = command.source.var;
		} else if (command.source.type == REGISTER_INDIRECT || command.source.type == REGISTER_DIRECT) {
			insert_operand_into_word(&(list[(*IC) + 1].word), command.source.value);
			set_ARE_into_word(&(list[(*IC) + 1].word), A);
		}
	}
	return 1;
}


int insert_symbol_address_into_words(word_data *words, int length, symbol_address symbol){
	int i;
	for (i = 0; i < length; ++i) {
		if (words[i].symbol != NULL) {
			if (strcmp(words[i].symbol, symbol.symbol_name) == 0){
				if (symbol.address != 1) {
					insert_operand_into_word(&words[i].word, symbol.address);
					set_ARE_into_word(&words[i].word, R);
				} else
					set_ARE_into_word(&words[i].word, E);
			}
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

void insert_value_into_word(word *word, int value){
	*word |= value & 0x7FFF;
}

void free_word_data_list(word_data *words, int length){
	int i;
	for (i = 0; i < length; ++i)
		if (words[i].symbol != NULL)
			free(words[i].symbol);
	
	free(words);
}


