#ifndef MMN14_DIRECTIVE_H
#define MMN14_DIRECTIVE_H

typedef enum directive_type{
    DATA,
    STRING,
    ENTRY,
    EXTERN
} directive_type;

directive_type get_directive_from_string(const char *str);

#endif //MMN14_DIRECTIVE_H
