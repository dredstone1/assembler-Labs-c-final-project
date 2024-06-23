#include "words_block.h"
#include <stdlib.h>
#include <string.h>

void handle_command_type(word_list_block *block, line_data *data);
void handle_directive_type(word_list_block *block, line_data *data);
int get_amount_of_words_from_command(line_command *command);
void add_word_node_to_block(word_list_block *block);
void insert_words_nodes_into_block(word_list_block *block, int amount_of_words);
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


void line_data_to_word_list_block(word_list_block *block, line_data *data){
    if (data->directive == NULL)
        handle_command_type(block, data);
    else
        handle_directive_type(block, data);
}

void handle_command_type(word_list_block *block, line_data *data){
    insert_words_nodes_into_block(block, get_amount_of_words_from_command(data->command));
    block->head->word = create_new_first_word(data);

    handle_operands_command(data->command, block);
}

word create_new_first_word(line_data *data){
    word new_word;
    int amount_of_operands = amount_of_variables_from_opcode(data->command->opcode);

    (new_word) = 0;
    set_opcode_into_word(&new_word, data->command->opcode);
    if (amount_of_operands == 2) {
        insert_operand_type_into_word(&new_word, SOURCE, data->command->variables[1].type);
        insert_operand_type_into_word(&new_word, DESTINATION, data->command->variables[0].type);
    }else {
        insert_operand_type_into_word(&new_word, DESTINATION, data->command->variables[0].type);
    }
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

void handle_directive_type(word_list_block *block, line_data *data){
    if (data->directive->type == ENTRY_ || data->directive->type == EXTERNAL)
        return;
    insert_words_nodes_into_block(block, data->directive->amount_of_variables);
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

void insert_words_nodes_into_block(word_list_block *block, int amount_of_words){
    for (int i=0; i<amount_of_words; i++)
        add_word_node_to_block(block);
}

void add_word_node_to_block(word_list_block *block){
    word_node *node = create_new_word_node(0);
    if (node == NULL) {
        /*error, memory allocation failed*/
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

void add_symbols_to_code_block(word_list_block *block, symbol_table *symbol_table) {
    word_node *current_node = block->head;
    symbol *symbol;

    while (current_node != NULL) {
        if (current_node->symbol[0] != '\0') {
            symbol = get_symbol_address_from_symbol_name(symbol_table, current_node->symbol);
            if (symbol == NULL) {
                // error symbol not found
                return;
            }

            insert_operand_into_word(&current_node->word, symbol->address);
            if (symbol->type == EXTERNAL) {
                set_ARE_into_word(&current_node->word, E);
            } else
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
