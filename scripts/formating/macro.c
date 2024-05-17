#include "macro.h"
#include <string.h>
#include <stdio.h>

bool is_ending_macro(const char line[]);
bool extra_char_at_end(const char line[], int loc);

bool is_line_macro(const char line[]){
    return strcmp(line, MACRO)==FALSE;
}

void create_new_macro(char macro_name[], line_node **node, macros *macros, error *error){
    macros->number_of_macros++;

    macros->macro = (macro*)realloc(macros->macro, macros->number_of_macros * sizeof(macro));
    if (macros->macro == NULL) {
        error->error_type = MEMORY_ALLOCATION_FAILED;
        return;
    }

    strcpy(macros->macro[macros->number_of_macros-1].macro_name, macro_name);

}

void set_macro_name(const char line[], macro *macro, pos *pos) {
    int i;
    bool found_text = FALSE;

    for (i=0; pos->column < LINE_SIZE; pos->column++) {
        if (line[pos->column] == '\0') {
            if (found_text) {
                macro->macro_name[i] = '\0';
                return;
            }

            /*error undefined macro name*/
        } if (line[pos->column] == ' ' || line[pos->column] == '\t') {
            if (!found_text)
                continue;

            /*error extra text after the macro name*/
        } else if (line[pos->column] == '\r')
            continue;
        else {
            macro->macro_name[i++] = line[pos->column];
            found_text = TRUE;
        }
    }
}


line_node **read_macro_lines(line_node **head) {
    line_node *next_node, **macro_node;

    macro_node = &(*head)->next;
    next_node = *head;

    while (next_node->next != NULL && is_ending_macro(next_node->next->line_text.content) == FALSE)
        next_node = next_node->next;

    *head = next_node->next->next;
    next_node->next = NULL;

    return macro_node;
}


bool is_ending_macro(const char line[]){
    int i;

    for (i = 0; 7 > i; i++)
        if (line[i] != END_MACRO[i])
            return FALSE;

    return TRUE;
}

macro* is_line_call_macro(macros *macros, line_node *node, error *error){
    int i,k=0;

    for (i = 0; macros->number_of_macros > i; i++) {
        while (node->line_text.content[k]==macros->macro[i].macro_name[k] || macros->macro[i].macro_name[k]=='\0') {
            if (macros->macro[i].macro_name[k] == '\0'){
                if (extra_char_at_end(node->line_text.content, k)==FALSE)
                    return &macros->macro[i];
                break;
            }
            k++;
        }
        k=0;
    }

    return NULL;
}

bool extra_char_at_end(const char line[], int loc){
    while (line[loc] == ' ' || line[loc] == '\t')
        loc++;

    if (line[loc] == '\0')
        return FALSE;

    return TRUE;
}

void push_to_macro(line_node **prev, line_node *new) {
    line_node *last_node, *new_copy, *next_node;

    if (*prev == NULL || new == NULL)
        return;

    /*Create a copy of the new node (and any nodes after it)*/
    new_copy = copy_block_of_nodes(&last_node, new);
    if (new_copy == NULL)
        return;

    /*Save the node that is currently next to the prev node*/
    next_node = (*prev)->next;

    /*Set the copied block to be the next block of the prev node*/
    (*prev)->next = new_copy;

    /*Set the originally next node to be the next node of the last node in the copied block*/
    last_node->next = next_node->next;
}
