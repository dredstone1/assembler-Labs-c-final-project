#include <stdio.h>
#include <stdlib.h>
#include "../systems/file.h"


/*
void format_line(file *file1, int line_number, error *error);
*/
void first_pass(file *file1, error *error);
/*
bool get_start_tag(line *line, error *error, pos *pos);
*/
void format_file(file *file1, error *error);
void post_formating(file *file1, error *error, macros *macros);
void post_formating_line(line_node *line_node, error *error, macros *macro_list, pos *pos);

int main(){
    file file1;
    error error;
    macros macros;

    char text[] = "macr m_macr\ncmp r3, #-6\nbne END\nendmacr";


    file1.pos.column = 0;
    file1.pos.line = 0;

    error.error_type = NOTHING;
    file1.number_of_rows = 0;
    file1.filename = "C:\\Users\\mayan\\Desktop\\shared\\mmn14_files\\ps2.as";

    read_file(&file1, &error);
    printf("number of rows: %d\n", file1.number_of_rows);
    post_formating(&file1, &error, &macros);
    printf("number of macros: %s\n", macros.macro[0].macro_name);
//    format_file(&file1, &error);

    return 0;
}



void format_file(file *file1, error *error){
    first_pass(file1, error);
}

void first_pass(file *file1, error *error){
    int i;

    for (i = 0; i < file1->number_of_rows; i++){
/*
        format_line(file1, i, error);
*/

        if (error->error_type != NOTHING){
            printf("oops\n");
            return;
        }
    }
}

void post_formating(file *file1, error *error, macros *macros){
    int i;
    line_node *node;

    node = file1->first_line;

    macros->number_of_macros = 0;

    for (i = 0; i < 5; i++){
        printf("line: %s\n", node->line_text.content);
/*
        printf("line: %s\n", node->next->line_text.content);
*/
        post_formating_line(node, error, macros, &file1->pos);
/*
        i = file1->pos.line;
*/
        node = node->next;
    }
}

void post_formating_line(line_node *line_node, error *error, macros *macro_list, pos *pos){
    if (is_line_macro(line_node->line_text.content, pos)==FALSE)
        return;

    macro_list->macro = (macro*)realloc(macro_list->macro, macro_list->number_of_macros* sizeof(macros));
    if (macro_list->macro == NULL){
        error->error_type = MEMORY_ALLOCATION_FAILED;
        return;
    }

    macro_list->number_of_macros++;
    set_macro_name(line_node->line_text.content, &macro_list->macro[macro_list->number_of_macros], pos);
}

/*
void format_line(file *file1, int line_number, error *error){
    get_start_tag(&file1->line[line_number], error, file1->pos);
}

bool get_start_tag(line *line, error *error, pos *pos) {
    int i;

    bool found_text = FALSE;
    line->tag.tag = FALSE;

    for (i = 0; i < MAX_TAG_SIZE; i++) {
        if (line->content[i] == ' ' || line->content[i] == '\t') {
            if (found_text)
                return FALSE;
        } else if (line->content[i] == '\n' || line->content[i] == '\0' || line->content[i] == '.')
            return FALSE;
        else if (line->content[i] == ':') {
            if (found_text) {
                line->tag.name[i] = '\0';
                line->tag.tag = TRUE;
                return TRUE;
            }

            error->error_type = UNDEFINED_TAG_NAME;
            return FALSE;
        } else {
            if (is_legal_char_tag(line->content[i]) == FALSE)
                return FALSE;

            found_text = TRUE;
            line->tag.name[i] = line->content[i];
        }
    }
    return FALSE;
}*/
