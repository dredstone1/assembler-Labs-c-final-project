#ifndef MMN14_FILE_H
#define MMN14_FILE_H

#include "utilities.h"
#include "error.h"
#include "word_conversion.h"
#include "symbol_table.h"


/**
 * @brief Writes the given lines of text to an .am file.
 * 
 * This function opens a new file with the .am extension and writes the provided lines of text to it.
 * If the file creation fails, the function will return without writing any data.
 * 
 * @param file_name The name of the file to write to.
 * @param lines An array of line_text structures containing the lines to write.
 * @param number_of_rows The number of lines to write.
 */
void write_to_file_am(char file_name[], line_text *lines, int number_of_rows);


/**
 * @brief Writes the given words to an object file.
 * 
 * This function writes the instruction counter (IC) and data counter (DC) to the first line of the file.
 * It then writes the instruction words from `list1` and the data words from `list2` to the file.
 * If the file creation fails, the function will return without writing any data.
 * 
 * @param list1 The first list of word data (instruction words).
 * @param list2 The second list of word data (data words).
 * @param fileName The name of the file to write to.
 * @param IC The instruction counter.
 * @param DC The data counter.
 */
void write_to_file_object(word_data *list1, word_data *list2, char fileName[], int IC, int DC);


/**
 * @brief Initializes a new file name.
 * 
 * This function allocates memory for the file name and copies the given base name to it.
 * It allocates memory for the length of the base name plus the length of the .xxx extension (FILE_NAME_ENDING_LENGTH).
 * If memory allocation fails, the function sets the error importance and returns.
 * 
 * @param file_name A pointer to the file name to initialize.
 * @param error A pointer to an error structure to handle memory allocation errors.
 * @param name The base name of the file.
 */
void initialize_new_file_name(char **file_name, error *error, char name[]);


/**
 * @brief Writes the labels marked as entries to an entry file.
 * 
 * This function writes the labels marked as entries to a new file with the .ent extension.
 * It first changes the file name to include the .ent extension. If there are no entries to write, the function returns immediately.
 * If the file creation fails, the function returns without writing any data.
 * For each entry, it searches for the corresponding label by name. If the label is found, it writes the label's name and address to the file.
 * 
 * @param entries The entries to write.
 * @param entry_amount The number of entries.
 * @param labels The labels to write.
 * @param label_amount The number of labels.
 * @param fileName The name of the file to write to.
 */
void write_to_file_entry(symbol_address *entries, int entry_amount, symbol_address *labels, int label_amount, char fileName[]);


/**
 * @brief Writes the words referring to external symbols to an external file.
 * 
 * This function writes the words that refer to an external symbols, to a new file with the .ext extension.
 * It first it changes the file name to include the .ext extension. If the file creation fails, the function returns immediately.
 * For each word, it checks if the word is marked as external. If it is, it writes the command's symbol and its address to the file.
 * 
 * @param commands The list of words to search for external symbols.
 * @param IC The instruction counter.
 * @param fileName The name of the file to write to.
 */
void write_to_file_external(word_data *commands, int IC, char fileName[]);
#endif