#include "../header/words_block.h"
#include <stdlib.h>
#include <string.h>

void handle_command_type(word_list_block *block, line_data *data, error_array *error, int line_number, char line[]);
void handle_directive_type(word_list_block *block, line_data *data, error_array *error);
int get_amount_of_words_from_command(line_command *command);
void add_word_node_to_block(word_list_block *block, error_array *error, int line_number, char line[]);
void insert_words_nodes_into_block(word_list_block *block, int amount_of_words, error_array *error, int line_number, char line[]);
word create_new_first_word(line_data *data);
void handle_operands_command(line_command *command, word_list_block *block);
void handle_operands_directive_list(line_directive *directive, word_list_block *block);

word_list_block* create_new_word_list_block(error_array *error){
    word_list_block *block = (word_list_block*)malloc(sizeof(word_list_block));
    if (block == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
        return NULL;
    }

    block->head = NULL;
    block->tail = NULL;
    block->size = 0;
    return block;
}

void line_data_to_word_list_block(word_list_block *block, line_data *data, error_array *error, int line_number, char line[]){
    if (data->directive == NULL)
        handle_command_type(block, data, error, line_number, line);
    else
        handle_directive_type(block, data, error);
}

void handle_command_type(word_list_block *block, line_data *data, error_array *error, int line_number, char line[]){
    insert_words_nodes_into_block(block, get_amount_of_words_from_command(data->command), error, line_number, line);
    block->head->word = create_new_first_word(data);

    handle_operands_command(data->command, block);
}

word create_new_first_word(line_data *data){
    word new_word;
    int amount_of_operands = amount_of_variables_from_opcode(data->command->opcode);

    new_word = 0;
    set_opcode_into_word(&new_word, data->command->opcode);
    if (amount_of_operands == 2) {
        insert_operand_type_into_word(&new_word, SOURCE, data->command->variables[1].type);
        insert_operand_type_into_word(&new_word, DESTINATION, data->command->variables[0].type);
    }else
        insert_operand_type_into_word(&new_word, DESTINATION, data->command->variables[0].type);
	
    set_ARE_into_word(&new_word, A);

    return new_word;
}

void handle_operands_command(line_command *command, word_list_block *block){
    word_node *current_node = block->head->next;
    int i = amount_of_variables_from_opcode(command->opcode)-1;

    while (current_node != NULL) {
        if (command->variables[i].type == IMMEDIATE) {
            insert_operand_into_word(&current_node->word, command->variables[i].value);
            set_ARE_into_word(&current_node->word, A);
        } else if (command->variables[i].type == DIRECT)
            strcpy(current_node->symbol, command->variables[i].symbol);
        else if (command->variables[i].type == REGISTER_INDIRECT || command->variables[i].type == REGISTER_DIRECT) {
            if (i==0)
                insert_operand_into_word(&current_node->word, command->variables[i].value);
            else
                insert_operand_into_word(&current_node->word, command->variables[i].value<<operand_bit_shift);

            if (command->variables[i-1].type > 1)
                insert_operand_into_word(&current_node->word, command->variables[i-1].value);

            set_ARE_into_word(&current_node->word, A);
        }
        current_node = current_node->next;
        i--;
    }
}

int get_amount_of_words_from_command(line_command *command){
    int amount = 1;
    amount += amount_of_variables_from_opcode(command->opcode);
    if (command->variables[0].type>1 && command->variables[1].type>1)
        amount--;
    return amount;
}

void handle_directive_type(word_list_block *block, line_data *data, error_array *error){
    if (data->directive->type == ENTRY || data->directive->type == EXTERN)
		return;
	
    insert_words_nodes_into_block(block, data->directive->amount_of_variables, error, 0, "");
	if (error->importance == CRITICAL)
		return;
	
	handle_operands_directive_list(data->directive, block);
}

void handle_operands_directive_list(line_directive *directive, word_list_block *block){
    word_node *current_node = block->head;
    int i = 0;

    while(current_node != NULL){
        insert_value_into_word(&current_node->word, directive->variables[i++]);
        current_node = current_node->next;
    }
}

void insert_words_nodes_into_block(word_list_block *block, int amount_of_words, error_array *error, int line_number, char line[]){
    int i;
	for (i=0; i<amount_of_words && error->importance!=CRITICAL; i++) {
		if (i==0)
			add_word_node_to_block(block, error, line_number, line);
		else
			add_word_node_to_block(block, error, line_number, line);
	}
}

void add_word_node_to_block(word_list_block *block, error_array *error, int line_number, char line[]){
    word_node *node = create_new_word_node(0, line_number, line, error);
    if (node == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
        return;
    }

    if (block->head == NULL) {
        block->head = node;
        block->tail = block->head;
    } else {
        block->tail->next = node;
        block->tail = block->tail->next;
    }
	
    block->size++;
}

void combine_word_list_blocks(word_list_block *block1, word_list_block *block2){
    if (block1->head == NULL) {
        block1->head = block2->head;
        block1->tail = block2->tail;
        block1->size = block2->size;
        return;
    }

    block1->tail->next = block2->head;
    block1->tail = block2->tail;
    block1->size += block2->size;
}

void add_symbols_to_code_block(word_list_block *block, symbol_table *symbol_table, error_array *error){
    word_node *current_node = block->head;
    symbol *symbol;

    while (current_node != NULL) {
        if (current_node->symbol[0] != '\0') {
            symbol = get_symbol_address_from_symbol_name(symbol_table, current_node->symbol);
            if (symbol == NULL) {
				add_error(error, SYMBOL_NOT_FOUND, current_node->line_number, strstr(current_node->line, current_node->symbol) - current_node->line, strstr(current_node->line, current_node->symbol) - current_node->line + strlen(current_node->symbol), WARNING, current_node->line, 0);
                return;
            }

            insert_operand_into_word(&current_node->word, symbol->address);
            if (symbol->type == EXTERNAL)
                set_ARE_into_word(&current_node->word, E);
            else
                set_ARE_into_word(&current_node->word, R);
        }

        current_node = current_node->next;
    }
}

void free_word_list_block(word_list_block *block){
    word_node *current_node = block->head;
    word_node *next_node;

    while (current_node != NULL) {
        next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
    free(block);
}

word_node* create_new_word_node(word word, int line_number, char *line, error_array *error){
	word_node *node = (word_node *)malloc(sizeof(word_node));
	if (node == NULL) {
		add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", 0);
		return NULL;
	}
	
	node->symbol[0] = '\0';
	node->line_number = line_number;
	node->line = line;
	node->word = word;
	node->next = NULL;
	return node;
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


