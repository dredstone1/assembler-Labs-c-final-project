#include "macro.h"
#include <string.h>
#include <stdlib.h>

bool is_ending_macro(const char line[]);
bool extra_char_at_end(const char line[], int loc);

bool is_line_macro(const char line[]){
    return strcmp(line, MACRO)==FALSE;
}

void add_macro(char macro_name[], line_node *node, macros *macros, error_array *error) {
    macros->number_of_macros++;
    macros->macro = realloc(macros->macro, (size_t)(macros->number_of_macros * sizeof(macro)));
    if (macros->macro == NULL) {
        add_error(error, MEMORY_ALLOCATION_FAILED, 0, 0, 0, CRITICAL, "", "", 0);
        free(macros->macro);
        return;
    }

    set_offset_line_node(node, 0);
    offset_line_node_by_i(node);

    strcpy(macros->macro[macros->number_of_macros - 1].macro_name, macro_name);
    macros->macro[macros->number_of_macros - 1].number_of_macro_lines = get_line_node_length(node);
    macros->macro[macros->number_of_macros - 1].macro_lines = node;
}

line_node *read_macro_lines(line_node **head) {
    line_node *next_node, *macro_node;

    macro_node = (*head)->next;
    next_node = *head;

    while (next_node->next != NULL && is_ending_macro(next_node->next->line_text.content) == FALSE)
        next_node = next_node->next;
    
    free(*head);
    *head = next_node->next->next;
    
    free(next_node->next);
    next_node->next = NULL;
    
    return macro_node;
}

void free_macros(macros *macros) {
    int i;
    for (i = 0; macros->number_of_macros > i; i++)
        free_line(macros->macro[i].macro_lines);

    free(macros->macro);
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
    char word[LINE_SIZE];

    skip_spaces_and_tabs(&k, node->line_text.content);
    get_next_word(word, &k, node->line_text.content, " \t\0", 3);

    if (extra_char_at_end(node->line_text.content, k)==TRUE)
        return NULL;

    for (i = 0; i < macros->number_of_macros; i++) {
        if (strcmp(word, macros->macro[i].macro_name)==FALSE)
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

    while (*node != NULL) {
        offset=0;
        get_next_word_n_skip(word, &offset, (*node)->line_text.content, " \t\0", 3);

        if (is_line_macro(word)==TRUE) {
            get_next_word(word, &offset, (*node)->line_text.content, " \t\0", 3);
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
