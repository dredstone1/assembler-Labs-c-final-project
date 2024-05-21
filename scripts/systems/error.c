#include "error.h"
#include "file.h"
#include <stdio.h>

void handel_error(error *error){

}

void print_error_pos(int line, int column){
    printf("(");
    print_pos(line, column);
    printf(")");
}
