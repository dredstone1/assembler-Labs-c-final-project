#include "formating.h"
#include "../file.h"
#include "../error.h"
#include "line_type.h"

void format_line(file *file1, int line_number, error *error);
void first_pass(file *file1, error *error);
void get_tag(line *line, error *error, pos *pos);

void format_file(file *file1, error *error){
    first_pass(file1, error);

}

void first_pass(file *file1, error *error){
    int i;

    for (i = 0; i < file1->number_of_rows; i++){
        format_line(file1, i, error);
        if (error->error_type != NOTHING)
            return;
    }
}

void format_line(file *file1, int line_number, error *error){
    if (is_empty(file1->line[line_number]))
        return;

    get_tag(file1->line, error, file1->pos);
}

void get_tag(line *line, error *error, pos *pos){
    int i;
    bool found_tag = FALSE;

    line->tag.tag = FALSE;

    for (i = 0; i < MAX_TAG_SIZE; i++) {
        if (line->content[i] == ' ' || line->content[i] == '\t') {
            if (!found_tag){
                return;
            }
        } else if (line->content[i] == '\n' || line->content[i] == '\0') {
            return;
        } else if (line->content[i] == ':') {
            if (found_tag){
                line->tag.name[i] = '\0';
                pos->column = i;
                line->tag.tag = TRUE;
            }
            return;
        } else {
            found_tag = TRUE;
            line->tag.name[i] = line->content[i];
        }
    }
}
