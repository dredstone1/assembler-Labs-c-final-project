#ifndef MMN14_POST_FORMATING_H
#define MMN14_POST_FORMATING_H

#include "file.h"

#define MACRO "macr"
#define END_MACRO "endmacr"
#define MACRO_MAX_LENGTH 77

typedef struct macro{
	line_text *lines;
	int number_of_macro_lines;
	char macro_name[MACRO_MAX_LENGTH];
} macro;


/**
 *
 */
void post_formating(error_array *error, char file_name[]);

#endif
