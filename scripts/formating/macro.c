#include "macro.h"

bool is_line_macro(char line[]){
    int i;
    bool found_text = FALSE;

    for (i = 0; i < MAX_TAG_SIZE; i++) {
        if (line[i] == ' ' || line[i] == '\t') {
            if (found_text)
                return FALSE;
        } else if (line[i] == '\n' || line[i] == '\0' || line[i] == '.')
            return FALSE;
        else if (line[i] == ':') {
            if (found_text) {
                return TRUE;
            }

            return FALSE;
        } else {
            if (is_legal_char_tag(line[i]) == FALSE)
                return FALSE;

            found_text = TRUE;
            macros macro_name.name[i] = line[i];
        }
    }
    return FALSE;
}

void add_macro_to_line(line *line, macros macros, int line_number){

}