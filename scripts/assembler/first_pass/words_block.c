#include "words_block.h"
#include <stdlib.h>

void handle_command_type(word_list_block *block, line_data *data);
void handle_directive_type(word_list_block *block, line_data *data);
int get_amount_of_words_from_command(line_command *command);
void add_word_node_to_block(word_list_block *block);
void insert_words_nodes_into_block(word_list_block *block, int amount_of_words);
word create_new_first_word(line_data *data);

word_list_block* create_new_word_list_block(){
    word_list_block *block = (word_list_block*)malloc(sizeof(word_list_block));
    if (block == NULL) {
        /*error, memory allocation failed*/
        return NULL;
    }

    block->head = NULL;
    block->tail = NULL;

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

/*
    handle_variables_command(data->command, block);
*/
}

word create_new_first_word(line_data *data){
    word new_word;
    (new_word) = 0;
    set_opcode_into_word(&new_word, data->command->opcode);

    insert_operand_type_into_word(&new_word, SOURCE, data->command->variables[1].type);
    insert_operand_type_into_word(&new_word, DESTINATION, data->command->variables[0].type);

    set_ARE_into_word(&new_word, A);

    return new_word;
}

int get_amount_of_words_from_command(line_command *command){
    int amount = 1;
    amount += amount_of_variables_from_opcode(command->opcode);

    if (command->variables[0].type>1 && command->variables[1].type >1)
        amount--;

    return amount;
}

void handle_directive_type(word_list_block *block, line_data *data){

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
}