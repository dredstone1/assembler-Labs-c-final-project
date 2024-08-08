#ifndef MMN14_FILE_H
#define MMN14_FILE_H

#include "utilities.h"
#include "error.h"
#include "word_conversion.h"
#include "symbol_table.h"


void write_to_file(char file_name[], line_text *lines, int number_of_rows);
void write_to_file_object(word_data *list1, word_data *list2, char fileName[], int IC, int DC);
void initialize_new_file_name(char **file_name, error *error, char name[]);
void set_ending_to_file_name(char fileName[], char ending[]);
void write_to_file_entry(symbol_address *entries, int entry_amount, symbol_address *labels, int label_amount, char fileName[]);
void write_to_file_external(word_data *commands, int IC, char fileName[]);
#endif
