#include "symbol.h"
#include <stdlib.h>
#include "../../data/line_node.h"
#include "../../data/line.h"
#include <string.h>


symbol* get_symbol(char line[], int *offset) {
    int i=0;
    symbol *symbol_obj = (symbol*)malloc(sizeof(symbol));
    char word[LINE_SIZE];
    symbol_obj->label[0] = '\0';
    get_next_word_n_skip(word, &i, line, " :\t \0", 4);
    if(line[i] != ':')
        return symbol_obj;

    strcpy(symbol_obj->label, word);

    *offset += i+1;
    return symbol_obj;
}
