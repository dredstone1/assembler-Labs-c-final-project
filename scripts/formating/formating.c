#include "formating.h"
#include "line_type.h"
#include <stdio.h>
#include "../systems/file.h"

void format_line(file *file1, int line_number, error *error);
void first_pass(file *file1, error *error);
bool get_start_tag(line *line, error *error, pos *pos);
void format_file(file *file1, error *error);

int main(){
    file file1;
    error error;

    error.error_type = NOTHING;
    file1.number_of_rows = 0;
    file1.filename = "../ps.as";
    read_file(&file1, &error);

    format_file(&file1, &error);

    return 0;
}



void format_file(file *file1, error *error){
    first_pass(file1, error);
}

void first_pass(file *file1, error *error){
    int i;

    for (i = 0; i < file1->number_of_rows; i++){
        format_line(file1, i, error);

        if (error->error_type != NOTHING){
            printf("oops\n");
            return;
        }
    }
}

void post_formating(file *file1, error *error){
    int i;

    for (i = 0; i < file1->number_of_rows; i++){

    }
}












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
}
