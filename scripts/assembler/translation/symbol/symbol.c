#include "symbol.h"
#include <stdlib.h>
#include <string.h>

symbol* get_symbol(char line[], int *offset) {
    int i=0;
    char word[LINE_SIZE];
    symbol *symbol_obj = (symbol*)malloc(sizeof(symbol));
    if (symbol_obj == NULL)
        return NULL;
    
    symbol_obj->label[0] = '\0';
    get_next_word_n_skip(word, &i, line, " :\t \0", 4);
    if(line[i] != ':')
        return symbol_obj;

    strcpy(symbol_obj->label, word);
    symbol_obj->type = START;

    *offset += i+1;
    return symbol_obj;
}
