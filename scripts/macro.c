#include "../header/macro.h"
#include "../header/utilities.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int is_ending_macro(const char line[]);

int is_line_macro(const char line[]){
    return strcmp(line, MACRO)==0;
}


void add_macro(char macro_name[], macro **macros, int *number_of_macros, error_array *error){
    number_of_macros++;
    *macros = (macro*)use_realloc(*macros, *number_of_macros * sizeof(macro), error);
    if (*macros == NULL)
		return;
	
    strcpy((*macros)[*number_of_macros - 1].macro_name, macro_name);
	(*macros)[*number_of_macros - 1].number_of_macro_lines = 0;
	(*macros)[*number_of_macros - 1].macro_lines = NULL;
}

line_node *read_macro_lines(line_node **head) {
    line_node *next_node, *macro_node;

    macro_node = (*head)->next;
    next_node = *head;

    while (next_node->next != NULL && is_ending_macro(next_node->next->line_text.content) == 0)
        next_node = next_node->next;
    
    free(*head);
    *head = next_node->next->next;
    
    free(next_node->next);
    next_node->next = NULL;
    
    return macro_node;
}

int is_ending_macro(const char line[]){
    int i;
    for (i = 0; 7 > i; i++)
        if (line[i] != END_MACRO[i])
            return 0;

    return 1;
}

void replace_line_to_macro(macro macro, line_node **node){
    line_node *temp = duplicate_lines_node(macro.macro_lines);
    add_node_to_end(temp,(*node)->next);
    set_offset_line_node(temp->next, (*node)->line_number);
    offset_line_node_by_i(temp->next);
    offset_line_node_by(temp->next, macro.number_of_macro_lines-2);
    free(*node);
    *node = temp->next;
}

void handle_macros(line_node **first_file_node,int *number_of_rows, macros *macros, error_array *error){
    line_node **node = first_file_node;
    macro *temp;
    int offset;
    char word[LINE_SIZE];

    macros->number_of_macros = 0;
    macros->macro = NULL;

    while (*node != NULL && error->importance != WARNING) {
        offset=0;
/*
        get_next_word_n_skip(word, &offset, (*node)->line_text.content, " \t\0", 3);
*/

        if (is_line_macro(word)==1) {
/*
            get_next_word(word, &offset, (*node)->line_text.content, " \t\0", 3);
*/
            add_macro(word, read_macro_lines(node), macros, error);
            number_of_rows -= macros->macro[macros->number_of_macros - 1].number_of_macro_lines + 2;
            offset_line_node_by(*node, -(macros->macro[macros->number_of_macros - 1].number_of_macro_lines+2));
        }
        if (macros->number_of_macros>0) {
            temp = get_macro_from_name(macros, *node);
            if (temp != NULL) {
                number_of_rows += temp->number_of_macro_lines;
                replace_line_to_macro(*temp, node);
            }
        }
        node = &(*node)->next;
    }
}
