#ifndef MMN14_SYMBOL_H
#define MMN14_SYMBOL_H

#define MAX_SYMBOL_SIZE 31

typedef enum symbol_type{
    START = 0,
    EXTERNAL = 3,
    ENTRY_ = 2
} symbol_type;

typedef struct symbol{
    symbol_type type;
    char label[MAX_SYMBOL_SIZE];
    int address;
} symbol;

symbol* get_symbol(char line[], int *offset);

#endif
