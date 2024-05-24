#include "symbol.h"
#include <stdlib.h>
#include "../../data/line_node.h"
#include "../../data/line.h"


char* get_symbol(char line[], int *offset) {
    int i=0;
    char *symbol = malloc(sizeof(char) * LINE_SIZE);

    get_next_word_n_skip(symbol, &i, line, " \t\0", 3);

    if(line[i-2] != ':') {
        return NULL;
    }
    *offset += i;
    return symbol;
}