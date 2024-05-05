#ifndef MMN14_LINE_TYPE_H
#define MMN14_LINE_TYPE_H

typedef enum sentence_type{
    NOTHING,
    EMPTY,
    COMMENT,
    DIRECTIVE,
    TEACHING
} sentence_type;

sentence_type get_line_type(line *line);

#endif //MMN14_LINE_TYPE_H
