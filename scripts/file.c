#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.h"


void read_file(file *file1, error *error){
    size_t bytesRead=1;
    char c;
    int i=0;

    size_t lenmax = sizeof(line)*LINE_JUMPER_SIZE;

    file1->line = malloc(lenmax);

    FILE *files = fopen(file1->filename, "r");
    if (files == NULL){
        error->error_message = FILE_NOT_FOUND;
        exit(1);
    }

    while (bytesRead != 0){
        bytesRead = fread(&c, sizeof(char), 1, files);

        if (c=='\n') {
            file1->line[file1->number_of_rows].content[i] = '\0';
            file1->line = realloc(file1->line, lenmax);
            file1->number_of_rows++;
            i=0;
            lenmax += sizeof(line)*LINE_JUMPER_SIZE;
        }else if(c=='\r')
            continue;
        else
            file1->line[file1->number_of_rows].content[i++] = c;
    }
    fclose(files);
}
