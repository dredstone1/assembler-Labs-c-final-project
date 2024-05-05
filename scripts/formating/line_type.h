#ifndef MMN14_LINE_TYPE_H
#define MMN14_LINE_TYPE_H
#include "../boolean.h"

typedef enum sentence_type{
    NOTHING,
    EMPTY,
    COMMENT,
    DIRECTIVE,
    TEACHING
} sentence_type;

bool is_empty(line *line);

#endif //MMN14_LINE_TYPE_H
