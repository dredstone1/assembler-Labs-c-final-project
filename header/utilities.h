#ifndef MMN14_UTILITIES_H
#define MMN14_UTILITIES_H

/*colors*/
#define BLACK_COLOR "\033[0m"
#define RED_COLOR "\x1b[38;2;247;84;100m"

typedef enum bool{
	FALSE,
	TRUE
} bool;

int int_to_octal(int num);
void get_next_word(char line_text[], int *offset, char line[], char separators[], int separators_amount);
void skip_spaces_and_tabs(int *offset, char line[]);

void get_next_word_n_skip(char line_text[], int *offset, char line[], char separators[], int separators_amount);
int count_char_until_not_separator(char line[], char c, int *offset, char separators[], int separators_amount);
bool extra_char_at_end(const char line[], int loc);

#endif
