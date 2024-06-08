#include "directive.h"
#include <string.h>

const char *directive_types_names[] = {
        ".data",
        ".string",
        ".entry",
        ".extern"
};

directive_type get_directive_from_string(const char *str) {
    directive_type code;
    for (code = DATA; code <= EXTERN; code++) {
        if (strcmp(str, directive_types_names[code]) == 0)
            return code;
    }
    return -1;
}