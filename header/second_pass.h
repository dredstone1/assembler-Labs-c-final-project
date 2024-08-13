#ifndef MMN14_SECOND_PASS_H
#define MMN14_SECOND_PASS_H

#include "word_conversion.h"


/**
 * @brief execute the second pass of the assembler
 * 
 * This function executes the second pass of the assembler.
 * it starts by setting the error importance to NO_ERROR.
 * then it check if there are any symbols duplication:
 *  - any symbol that is defined as a entry and as an external, if there are,
 *  		it will print the appropriate error.
 *  - any symbol that is defined more than once:
 * 			external symbol are found in the symbol table,
 * 			so it will check if there are any external symbols that are defined more than once,
 * 			as an external symbol or as an label.
 * 			if there are, it will print the appropriate error.
 * 			
 * then if there isn't any error,
 * it will insert into word with a symbol the corresponding address from the symbol table,
 * and if it doesn't find the symbol in the symbol table, it will print the appropriate error.
 * 
 * @param commands The word data list.
 * @param entries The entries to check for duplicates.
 * @param symbol_table The symbol table containing the symbols.
 * @param error The error structure.
 * @param IC The instruction counter.
 * @param label_amount The number of labels in the symbol table.
 * @param entry_amount The number of entries.
 * @param extern_amount The number of external symbols.
 */
void second_pass(word_data *commands, symbol_address *entries, symbol_address *symbol_table, error *error, int IC,
				 int label_amount, int entry_amount, int extern_amount);
#endif
