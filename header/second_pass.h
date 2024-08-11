#ifndef MMN14_SECOND_PASS_H
#define MMN14_SECOND_PASS_H

#include "word_conversion.h"

void second_pass(word_data *commands, symbol_address *entries, symbol_address *symbol_table, error *error, int IC,
				 int label_amount, int entry_amount, int extern_amount);
#endif
