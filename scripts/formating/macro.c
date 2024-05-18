#include "macro.h"
#include <string.h>
#include <stdio.h>

bool is_ending_macro(const char line[]);
bool extra_char_at_end(const char line[], int loc);

bool is_line_macro(const char line[]){
    return strcmp(line, MACRO)==FALSE;
}

void add_macro(char macro_name[], line_node **node, macros *macros) {
    macros->number_of_macros++;
    macros->macro = realloc(macros->macro, (size_t)(macros->number_of_macros * sizeof(macro)));
    if (macros->macro == NULL) {
        // error not enough memory
        return;
    }

    // Update the newly allocated macro
    strcpy(macros->macro[macros->number_of_macros - 1].macro_name, macro_name);
    macros->macro[macros->number_of_macros - 1].number_of_macro_lines = get_line_node_length(*node);
    macros->macro[macros->number_of_macros - 1].macro_lines = *node;
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

macro* get_macro_from_name(macros *macros, line_node *node){
    int i,k=0;
    line_text word;

    skip_spaces_and_tags(&k, node->line_text.content);
    get_next_word(&word, &k, node->line_text.content, " \t\0", 3);

    if (extra_char_at_end(node->line_text.content, k)==TRUE)
        return NULL;

    for (i = 0; macros->number_of_macros > i; i++) {
        if (strcmp(word.content, macros->macro[i].macro_name)==FALSE)
            return &macros->macro[i];
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

void replace_line_to_macro(macro macro, line_node **node){
    line_node *temp;
    temp = duplicate_lines_node(macro.macro_lines);
    add_node_to_end(temp,(*node)->next);
    *node = temp;
}
