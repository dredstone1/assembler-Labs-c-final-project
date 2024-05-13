#include "directive_line_type.h"
#include <stdio.h>

bool is_directive_type_is(char line[], int *offset, char compare[]){
    int i = *offset;

    printf("iii: %s ,,, %s\n", line, compare);
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
