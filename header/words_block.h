#ifndef MMN14_WORDS_BLOCK_H
#define MMN14_WORDS_BLOCK_H

#include "symbol_table.h"
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

typedef struct word_node{
	word word;
	char symbol[MAX_SYMBOL_SIZE];
	int line_number;
	char *line;
	struct word_node *next;
} word_node;

typedef struct word_list_block{
	word_node *head;
	word_node *tail;
	int size;
} word_list_block;

void line_data_to_word_list_block(word_list_block *block, line_data *data, error_array *error, int line_number, char line[]);
word_list_block* create_new_word_list_block(error_array *error);
void combine_word_list_blocks(word_list_block *block1, word_list_block *block2);
void add_symbols_to_code_block(word_list_block *block, symbol_table *symbol_table, error_array *error);
void free_word_list_block(word_list_block *block);
void set_opcode_into_word(word *word, opcode op_code);
void insert_operand_type_into_word(word *word, operand op, variable_type type);
void set_ARE_into_word(word *word, ARE are);
void insert_operand_into_word(word *word, int value);
void insert_value_into_word(word *word, int value);
word_node* create_new_word_node(word word, int line_number, char *line, error_array *error);

#endif
