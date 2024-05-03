#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void read_file(char *filename, char **file_content, int *size){
    char c;
    FILE *file = fopen(filename, "r");
    if (file == NULL){
        printf("Error: File not found\n");
        exit(1);
    }
    *file_content = NULL;
    *size = 0;
    while ((c = fgetc(file)) != EOF){
        *file_content = (char *)realloc(*file_content, *size + 1);
        (*file_content)[*size] = c;
        (*size)++;
    }
    *file_content = (char *)realloc(*file_content, *size + 1);
    (*file_content)[*size] = '\0';
    fclose(file);
}