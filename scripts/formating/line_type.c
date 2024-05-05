#include "line_type.h"
#include "../file.h"

int is_line_type_empty(line *line);

sentence_type get_line_type(char *line){
    if (is_line_type_empty(line)){
        return EMPTY;
    }

    return NOTHING;
}

int is_line_type_empty(line *line){
    int i;
    for (i = 0; i < LINE_SIZE; ++i) {
        if (line->content[i] != ' ' && line[i] != '\t' && line[i] != '\n' && line[i] != '\0'){
            return 0;
        }
    }
}