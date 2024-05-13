#include "directive_line_type.h"

bool is_directive_type_is(char line[], int *offset, char compare[]){
    int i = 0;

    for (; i < MAX_DIRECTIVE_SIZE+1; i++) {
        if(compare[i] == '\0'){
            *offset += i-1;
            return TRUE;
        }

        if (line[i+*offset] != compare[i])
            return FALSE;
    }

    return FALSE;
}
