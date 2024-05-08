#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.h"
void reset_content(line *line);
void read_file(file *file1, error *error) {
    size_t bytesRead = 1;
    char c;
    int i = 0;
    bool found = FALSE;
    size_t lenmax = sizeof(line) * LINE_JUMPER_SIZE;

    file1->line = malloc(lenmax);

    if (file1->line == NULL) {
        error->error_type = MEMORY_ALLOCATION_FAILED;
        return;
    }

    FILE *files = fopen(file1->filename, "r");
    if (files == NULL) {
        error->error_type = FILE_NOT_FOUND;
        return;
    }
    while (bytesRead != 0) {
        bytesRead = fread(&c, sizeof(char), 1, files);
/*
        printf("test: %c\n", c);
*/

        if (c == '\n') {
            if (found == TRUE) {
                file1->line[file1->number_of_rows].content[i] = '\0';

                found = FALSE;
                file1->number_of_rows++;
                i = 0;
            }
        } else if (c == '\r')
            continue;
        else if (c == ' ' || c == '\t') {
            if (found == TRUE)
                file1->line[file1->number_of_rows].content[i++] = c;
        } else {

            if (found == FALSE) {
/*
                printf("found\n");
*/
                lenmax += sizeof(line) * LINE_JUMPER_SIZE;
                file1->line = realloc(file1->line, lenmax);
                if (file1->line == NULL) {
                    error->error_type = MEMORY_ALLOCATION_FAILED;
                    return;

                }
                i = 0;
                reset_content(&file1->line[file1->number_of_rows]);
            }

            found = TRUE;
            file1->line[file1->number_of_rows].content[i++] = c;
        }
    }
    fclose(files);
}

void print_pos(pos pos) {
    printf("%d, %d", pos.line, pos.column);
}

void reset_content(line *line) {
    int i;
    for (i = 0; i < LINE_SIZE; i++) {
        line->content[i] = ' ';
    }
}