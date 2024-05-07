#include "error.h"
#include "file.h"
#include <stdio.h>

void handel_error(error *error){

}

void print_error_pos(pos pos){
    printf("(");
    print_pos(pos);
    printf(")");
}

void print_line(line line){
    printf("%s", line.content);
}

