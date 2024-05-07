#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.h"

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

        if (c == '\n') {
            file1->line[file1->number_of_rows].content[i] = '\0';

            file1->line = realloc(file1->line, lenmax);
            if (file1->line == NULL) {
                error->error_type = MEMORY_ALLOCATION_FAILED;
                return;
            }
            found = FALSE;
            file1->number_of_rows++;
            i = 0;
        } else if (c == '\r')
            continue;
        else if (c == ' ' || c == '\t') {
            if (found == TRUE)
                file1->line[file1->number_of_rows].content[i++] = c;
        } else {
            if (found == FALSE) {
                lenmax += sizeof(line) * LINE_JUMPER_SIZE;
                bytesRead = fread(&c, sizeof(char), 1, files);
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