#include <stdio.h>
#include <stdlib.h>
#include "../systems/file.h"

void first_pass(file *file1, error *error);
void get_start_tag(line_node *node, error *error);
void post_formating(file *file1, error *error, macros *macros);
void format_line(line_node *node, error *error, pos *pos);
void set_line_type(line_node *node, error *error);

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
    first_pass(&file1, &error);

    free_file_lines(&file1);
    return 0;
}


void first_pass(file *file1, error *error){
    line_node *temp = file1->first_line;

    add_data_object_to_lines(temp);

    while (temp != NULL) {
        format_line(temp, error, &file1->pos);

        temp = temp->next;
    }
}

void post_formating(file *file1, error *error, macros *macros) {
    line_node *node, *prev_node;
    macro *temp;
    bool last_macro = FALSE;

    node = file1->first_line;
    macros->number_of_macros = 0;

    while (node != NULL) {
        if (is_line_macro(node->line_text.content, &file1->pos)) {
            macros->number_of_macros++;

            macros->macro = (macro*)realloc(macros->macro, macros->number_of_macros * sizeof(macro));
            if (macros->macro == NULL) {
                error->error_type = MEMORY_ALLOCATION_FAILED;
                return;
            }

            set_macro_name(node->line_text.content, &macros->macro[macros->number_of_macros-1], &file1->pos);
            get_macro_lines(&(prev_node->next), &macros->macro[macros->number_of_macros-1], error);

            node = prev_node->next;
        }

        if(macros->number_of_macros>0) {
            temp = is_line_call_macro(macros, node, error);
            if (temp != NULL) {
                push_to_macro(&prev_node, temp->macro_lines);
                last_macro = TRUE;
            }
        }

        if(last_macro)
            node = prev_node->next;

        prev_node = node;
        file1->pos.line++;
        node = node->next;
        temp= NULL;
    }
}


void format_line(line_node *node, error *error, pos *pos){
    get_start_tag(node, error);
    set_line_type(node, error);

    if (node->line_data->directive == TRUE)
        set_direct_line_type(node);
}




void get_start_tag(line_node *node, error *error) {
    int i;
    bool found_text = FALSE;

    node->line_data->tag.tag = FALSE;
    node->line_data->offset = 0;

    for (i = 0; i < MAX_TAG_SIZE; i++) {
        if (node->line_text.content[i] == ' ' || node->line_text.content[i] == '\t') {
            if (found_text)
                return;
        } else if (node->line_text.content[i] == '\n' || node->line_text.content[i] == '\0' || node->line_text.content[i] == '.')
            return;
        else if (node->line_text.content[i] == ':') {
            if (found_text) {
                node->line_data->tag.name[i] = '\0';
                node->line_data->tag.tag = TRUE;
                node->line_data->offset = i+1;
                return;
            }

            error->error_type = UNDEFINED_TAG_NAME;
            return;
        } else {
            if (is_legal_char_tag(node->line_text.content[i]) == FALSE)
                return;

            found_text = TRUE;
            node->line_data->tag.name[i] = node->line_text.content[i];
        }
    }
    return;
}

void set_line_type(line_node *node, error *error){
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
