#include "words_block.h"
#include <stdlib.h>
#include <stdio.h>

void handle_command_type(word_list_block *block, line_data *data);
void handle_directive_type(word_list_block *block, line_data *data);
int get_amount_of_words_from_command(line_command *command);
void add_word_node_to_block(word_list_block *block);
void insert_words_nodes_into_block(word_list_block *block, int amount_of_words);
word create_new_first_word(line_data *data);
void handle_operands_command(line_command *command, word_list_block *block);
void handle_operands_directive_list(line_directive *directive, word_list_block *block);
void handle_operands_directive_single(line_directive *directive, word_list_block *block);

word_list_block* create_new_word_list_block(){
    word_list_block *block = (word_list_block*)malloc(sizeof(word_list_block));
    if (block == NULL) {
        /*error, memory allocation failed*/
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
    (new_word) = 0;
    set_opcode_into_word(&new_word, data->command->opcode);

    insert_operand_type_into_word(&new_word, SOURCE, data->command->variables[0].type);
    insert_operand_type_into_word(&new_word, DESTINATION, data->command->variables[1].type);

    set_ARE_into_word(&new_word, A);

    return new_word;
}

void handle_operands_command(line_command *command, word_list_block *block){
    word_node *current_node = block->head->next;
    int i = 0;

    while (current_node != NULL) {
        if (command->variables[i].type == 0) {
            insert_operand_into_word(&current_node->word, command->variables[i].value);
            set_ARE_into_word(&current_node->word, A);
        } else if (command->variables[i].type == 1) {
            insert_operand_into_word(&current_node->word, command->variables[i].value);
        } else if (command->variables[i].type == 2 || command->variables[i].type == 3) {
            if (i==1)
                insert_operand_into_word(&current_node->word, command->variables[i].value);
            else
                insert_operand_into_word(&current_node->word, command->variables[i].value<<operand_bit_shift);

            if (current_node->next == NULL) {
                insert_operand_into_word(&current_node->word, command->variables[i + 1].value<<operand_bit_shift);
                set_ARE_into_word(&current_node->word, A);
            }

            set_ARE_into_word(&current_node->word, A);
        }
        current_node = current_node->next;

        i++;
    }
}

int get_amount_of_words_from_command(line_command *command){
    int amount = 1;
    amount += amount_of_variables_from_opcode(command->opcode);
    if (command->variables[0].type>1 && command->variables[1].type >1)
        amount--;
    return amount;
}

void handle_directive_type(word_list_block *block, line_data *data){
    if (data->directive->type == DATA || data->directive->type == STRING) {
        insert_words_nodes_into_block(block, data->directive->amount_of_variables);
        handle_operands_directive_list(data->directive, block);
    }
}

void handle_operands_directive_list(line_directive *directive, word_list_block *block){
    word_node *current_node = block->head;
    int i = 0;

    while(current_node != NULL){
        current_node->word = directive->variables[i++];
        current_node = current_node->next;
    }
}

void handle_operands_directive_single(line_directive *directive, word_list_block *block){
    word_node *current_node = block->head;
    int i = 0;

    while(current_node != NULL){
        current_node->word = directive->variable.value;
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

void combine_word_list_blocks(word_list_block *block1, word_list_block *block2, char line[LINE_SIZE]){
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