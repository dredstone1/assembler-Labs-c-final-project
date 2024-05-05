#include "formating.h"
#include "../file.h"
#include "../error.h"
#include "line_type.h"

void format_line(file *file1, int line_number, error *error);
void first_pass(file *file1, error *error);

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
}

void get_tag(line *line, error *error){
    int i;
    char tag[LINE_SIZE];
    bool found_tag = FALSE;

    for (i = 0; i < LINE_SIZE; i++){
        if (line->content[i] == ' ' || line->content[i] == '\t') {
            if (found_tag){
                tag[i] = '\0';
                line->sentence_type = tag;
                return;
            }
        }else if (line->content[i] == '\n' || line->content[i] == '\0') {
            return;
        }else if (line->content[i] == ':'){
            tag[i] = '\0';
            line->sentence_type = tag;
            return;

        }else {
                found_tag = TRUE;
                tag[i] = line->content[i];
            }
        }
    }
    tag[i] = '\0';
    if (tag[i - 1] == ':'){
        tag[i - 1] = '\0';
        line->sentence_type = tag;
    }
}
