#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.h"
void free_line(line_node *node);

void read_file(file *file1, error *error) {
    size_t bytesRead = 1;
    char c;
    int i = 0, line_number = 0;
    bool found = FALSE;
    line_node *last_line;

    FILE *files = fopen(file1->filename, "r");

    last_line = create_line_node(NULL, NULL);
    file1->first_line = last_line;

    if (files == NULL) {
        error->error_type = FILE_NOT_FOUND;
        return;
    }

    while (bytesRead != 0) {
        bytesRead = fread(&c, sizeof(char), 1, files);

        if (c == '\n') {
            line_number++;
            if (found == TRUE) {
                last_line->line_text.content[i] = '\0';

                found = FALSE;
                i = 0;
            }
        } else if (c == '\r')
            continue;
        else if (c == ' ' || c == '\t') {
            if (found == TRUE)
                last_line->line_text.content[i++] = c;
        } else {
            if (found == FALSE){
                if (file1->number_of_rows>0)
                    last_line = last_line->next = create_line_node(NULL, NULL);

                i = 0;
                file1->number_of_rows++;
                last_line->line_number = line_number;
            }

            found = TRUE;
            last_line->line_text.content[i++] = c;
        }
    }

    fclose(files);
}

void print_pos(pos pos) {
    printf("%d, %d", pos.line, pos.column);
}

void free_file_lines(file *file1){
    free_line(file1->first_line);
}

void free_line(line_node *node){
    if (node == NULL)
        return;

    free_line(node->next);

    free(node->line_data);
    free(node);
}
