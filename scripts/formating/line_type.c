#include "line_type.h"
#include "../file.h"

bool is_line_type_empty(line *line);
bool is_line_type_comment(line *line);

bool is_empty(line *line){
    if (is_line_type_empty(line))
        return TRUE;

    if (is_line_type_comment(line))
        return TRUE;

    return FALSE;
}

bool is_line_type_empty(line *line){
    int i;

    for (i = 0; i < LINE_SIZE; ++i) {
        if (line->content[i] == ' ' || line->content[i] == '\t')
            continue;
        else if (line->content[i] == '\n' || line->content[i] == '\0')
            return TRUE;
        else
            return FALSE;
    }
    return FALSE;
}

bool is_line_type_comment(line *line){
    return line->content[0] == ';';
}

bool is_line_type_directive(line *line){
    return line->content[0] == ';';
}