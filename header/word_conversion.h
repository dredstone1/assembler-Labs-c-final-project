#ifndef MMN14_WORD_CONVERSION_H
#define MMN14_WORD_CONVERSION_H

#include "symbol_table.h"
#include "consts.h"
#include "line_data.h"

#define opcode_bit_shift 11
#define operand_bit_shift 3
#define operand_bit_size 4

typedef enum operand{
	DESTINATION,
	SOURCE
} operand;

typedef enum ARE{
	E,
	R,
	A
} ARE;

typedef unsigned short word;

typedef struct word_data{
	word word;
	char *symbol;
	int line_number;
} word_data;


void set_ARE_into_word(word *word, ARE are);


void set_opcode_into_word(word *word, opcode op_code);


void insert_operand_type_into_word(word *word, operand op, variable_type type);


void insert_operand_into_word(word *word, int value);


void insert_value_into_word(word *word, int value);

/**
 * @brief add instruction to words
 * 
 * @param list
 * @param instruction
 * @param line_number
 * @param error
 * @return 1 if success, 0 if fail
 */
int add_instruction_to_words(word_data *list, instruction_data instruction, int line_number, error_array *error, int *DC);


/**
 * @brief add data to words
 * 
 * @param list
 * @param data
 * @param line_number
 * @param error
 * @return 1 if success, 0 if fail
 */
int add_command_to_words(word_data *list, command_data command, int line_number, error_array *error, int *IC);


/**
 * 
 */
int get_amount_of_words_from_command(command_data command);



int insert_symbol_address_into_words(word_data *words, int length, symbol_address symbol);


/**
 * @brief free word data list
 * 
 * @param words
 * @param length
 */
void free_word_data_list(word_data *words, int length);
#endif
