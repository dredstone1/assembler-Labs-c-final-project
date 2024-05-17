#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../systems/file.h"
#include "other.h"

void first_pass(file *file1);
void get_start_tag(line_node *node);
void post_formating(file *file1, error *error, macros *macros);
void format_line(line_node *node);
void set_line_type(line_node *node);

int main(){
    file file1;
    error error;
    macros macros;

    file1.pos.column = 0;
    file1.pos.line = 0;

    error.error_type = NOTHING;
    file1.number_of_rows = 0;
    file1.filename = "C:\\Users\\mayan\\Desktop\\shared\\mmn14_files\\ps2.as";

    read_file(&file1, &error);

    post_formating(&file1, &error, &macros);
    first_pass(&file1);

    free_file_lines(&file1);
    return 0;
}


void first_pass(file *file1){
    line_node *temp = file1->first_line;

    add_data_object_to_lines(temp);

    while (temp != NULL) {
        format_line(temp);

        temp = temp->next;
    }
}

void post_formating(file *file1, error *error, macros *macros) {
    line_node **node, *prev_node, **new_block, **temp2;
    macro *temp;
    line_text word;

    int offset;


    node = &file1->first_line;
    macros->number_of_macros = 0;

    while (*node != NULL) {
        offset=0;
        get_next_word(&word, &offset, (*node)->line_text.content, " \t\0", 3);

        if (is_line_macro(word.content)==TRUE) {

            skip_spaces_and_tags(&offset, (*node)->line_text.content);

            get_next_word(&word, &offset, (*node)->line_text.content, " \t\0", 3);

            new_block = read_macro_lines(node);

            create_new_macro(word.content, new_block, macros, error);

        }

        if(macros->number_of_macros>0) {
            temp = is_line_call_macro(macros, *node, error);
            if (temp != NULL) {
                push_to_macro(&prev_node, temp->macro_lines);
            }
        }

        file1->pos.line++;
        node = &(*node)->next;
        temp= NULL;
    }
}


void format_line(line_node *node){
    node->line_data->offset = 0;

    skip_spaces_and_tags(&node->line_data->offset, node->line_text.content);
    get_start_tag(node);
    set_line_type(node);

    if (node->line_data->directive == TRUE)
        set_direct_line_type(node);
    else
        set_operation_line_type(node);
}




void get_start_tag(line_node *node) {
    int i=0;
    node->line_data->tag.tag = FALSE;
    while (is_legal_char_tag(node->line_text.content[i]) == TRUE) {
        node->line_data->tag.name[i] = node->line_text.content[i];
        i++;
    }

    if (node->line_text.content[i] == ':') {
        node->line_data->tag.name[i] = '\0';
        node->line_data->tag.tag = TRUE;
        node->line_data->offset = i+1;
    }
}

void set_line_type(line_node *node){
    int i;

    node->line_data->directive = FALSE;

    for (i = node->line_data->offset; i < MAX_TAG_SIZE; i++) {
        if (node->line_text.content[i] == ' ' || node->line_text.content[i] == '\t')
            continue;
        else if (node->line_text.content[i] =='.') {
            node->line_data->directive = TRUE;
            node->line_data->offset = i+1;
        }

        return;
    }
}
