#include "directive_line_type.h"

bool is_directive_type_is(char line[], int *offset, char compare[]){
    int i = *offset;

    for (; i < MAX_DIRECTIVE_SIZE; i++) {
        if(compare[i] == '\0'){
            *offset = i;
            return TRUE;
        }

        if (line[i] != compare[i])
            return FALSE;
    }
    return FALSE;
}
