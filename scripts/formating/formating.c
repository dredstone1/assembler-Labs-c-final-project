#include <stdio.h>
#include "../systems/file.h"

void first_pass(file *file1);
void get_start_tag(line_node *node);
void post_formating(file *file1, macros *macros);
void format_line(line_node *node);
void set_line_type(line_node *node);

int main(){
    file file1;
    error error;
    macros macros;

    char *combined;
    file1.pos.column = 0;
    file1.pos.line = 0;

    error.error_type = NOTHING;
    file1.number_of_rows = 0;
    file1.filename = "C:\\Users\\mayan\\Desktop\\shared\\mmn14_files\\ps2.as";
    read_file(&file1, &error);

    post_formating(&file1, &macros);
    printf("number of rows: %d\n", file1.number_of_rows);
    combined = combine_line_nodes_text(file1.first_line, file1.number_of_rows);
    create_n_write_file(combined, "C:\\Users\\mayan\\Desktop\\shared\\mmn14_files\\ps2.am");
    free(combined);

/*    first_pass(&file1);*/
    printf("eeee");
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

void post_formating(file *file1, macros *macros) {
    line_node **node = &file1->first_line, **new_block;
    macro *temp;
    int offset;
    line_text word;

    macros->number_of_macros = 0;
    macros->macro = NULL;

    while (*node != NULL) {
        offset=0;

        skip_spaces_and_tags(&offset, (*node)->line_text.content);
        get_next_word(&word, &offset, (*node)->line_text.content, " \t\0", 3);
        skip_spaces_and_tags(&offset, (*node)->line_text.content);

        if (is_line_macro(word.content)==TRUE) {
            get_next_word(&word, &offset, (*node)->line_text.content, " \t\0", 3);
            new_block = read_macro_lines(node);
            add_macro(word.content, new_block, macros);
            printf("macro name: %d\n", macros->number_of_macros);
            file1->number_of_rows -= macros->macro[macros->number_of_macros].number_of_macro_lines;
        }

        if(macros->number_of_macros>0) {
            temp = get_macro_from_name(macros, *node);

            if (temp != NULL) {
                file1->number_of_rows += temp->number_of_macro_lines-1;
                replace_line_to_macro(*temp, node);
            }
        }

        file1->pos.line++;
        node = &(*node)->next;
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
