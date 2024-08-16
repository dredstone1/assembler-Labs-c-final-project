#ifndef MMN14_WORD_CONVERSION_H
#define MMN14_WORD_CONVERSION_H

#include "consts.h"
#include "line_data.h"

/*the bit in the word for the opcode*/
#define opcode_bit_shift 11

/*the bit in the word for the operands type to start*/
#define operand_bit_shift 3

/*the length of each operand type*/
#define operand_bit_size 4

/*a mask to make sure value is 15 bits long*/
#define SHORT_MAX_BIT_SHIFT 32767


/**
 * @brief Enum representing the operand type.
 * 
 * This enum represents the type of an operand in command.
 */
typedef enum operand {
	DESTINATION, /*Destination operand*/
	SOURCE /*Source operand*/
} operand;


/**
 * @brief Enum representing the ARE (Absolute, Relocatable, External) type.
 * 
 * This enum represents the three possible types of a word in the machine code:
 */
typedef enum ARE {
	E, /*External*/
	R, /*Relocatable*/
	A  /*Absolute*/
} ARE;


/**
 * @brief Type definition for a word, representing a 16-bit value.
 */
typedef unsigned short word;


/**
 * @brief Structure representing word data.
 * 
 * This structure represents a word in the machine code, containing the word value,
 * and the associated symbol(if any), line number, and external flag.
 */
typedef struct word_data {
	word word; /*The word value*/
	char *symbol; /*Associated symbol, if any*/
	int line_number; /*Line number in the source file*/
	int external; /*Flag indicating if the word is external*/
} word_data;


/**
 * @brief Sets the ARE field in a word.
 *
 * This function encodes an addressing mode (ARE) and inserts it
 * into a specific position within a word, by shifting '1' left by the ARE value.
 * 
 * @param word Pointer to the word to modify.
 * @param ARE The ARE value to set.
 */
void set_ARE_into_word(word *word, ARE ARE);


/**
 * @brief Sets the opcode within a word.
 *
 * This function takes an opcode value and inserts it into a 
 * designated position within a word.
 * 
 * @param word  A pointer to the word where the opcode will be set.
 * @param opcode The opcode to insert into the word.
 */
void set_opcode_into_word(word *word, opcode opcode);


/**
 * Inserts operand type information into a specified first word.
 *
 * This function encodes the type of the operand by shifting '1' by the type,
 * then shifting the result by operand_bit_shift + (operand_bit_size * operand),
 * then it calls insert_value_into_word to insert the new value into the word.
 *
 * @param word A pointer to the data word to modify.
 * @param operand The type of op
 * @param type The type of the operand to encode. 
 */
void insert_operand_type_into_word(word *word, operand operand, variable_type type);


/**
 * @brief Inserts an operand value into a word.
 *
 * This function takes a pointer to a word and an integer value, then inserts the operand value into the word
 * by shifting the value left by `operand_bit_shift` bits and calling `insert_value_into_word`.
 *
 * @param word Pointer to the word where the operand value will be inserted.
 * @param value The integer operand value to be inserted into the word.
 */
void insert_operand_into_word(word *word, int value);


/**
 * @brief Inserts a short value into a word, preserving existing bits.
 *
 * This function takes a pointer to a word and a short value as input. It
 * inserts the short value into the word, preserving the existing bits of the word
 * and only modifying the bits relevant to the short value.
 *
 * @param word A pointer to the word to modify.
 * @param value The short value to insert.
 */
void insert_value_into_word(word *word, short value);


/**
 * @brief Adds an directive to the word list, encoding its values.
 * 
 * This function takes an directive structure,
 * iterates through the variables in the data,
 * and resets the corresponding word in the word list,
 * and inserts the data values.
 * It updates the data counter (DC) accordingly.
 *
 * @param list Pointer to the array of word_data structures representing the machine code.
 * @param directive The directive_data structure containing the values to be encoded.
 * @param line_number The line number of the directive in the assembly source file, used for error reporting.
 * @param DC Pointer to the current data counter (DC), which is incremented as words are added.
 */
int add_data_to_words(word_data **list, directive_data directive, int line_number, int *DC, error *error);


/**
 * @brief Adds a command to the word list.
 * 
 * This function takes a command structure, extracts its opcode, operand types, and values,
 * and encodes them into one to three words in the word list. It also handles symbol association for
 * direct addressing modes and updates the instruction counter (IC).
 *
 *
 * 
 * The function first determines the number of operands based on the opcode. It then resets the word at
 * the current IC and potentially the following two words, depending on the number of operands. 
 * 
 * It then encodes the opcode and operand types into the first word. If there are two operands, it 
 * encodes the source and destination types. If there's only one operand, it encodes the destination type.
 * The ARE field of the first word is always set to 'A' (Absolute) initially.
 * 
 * Next, the function handles operand value encoding based on the addressing mode:
 * 
 * -IMMEDIATE: The operand value is directly encoded into the word.
 * -DIRECT: The symbol representing the label is stored for later address resolution.
 * -REGISTER_DIRECT/REGISTER_INDIRECT: The register value is encoded into the word. 
 * 
 * If the source and destination operands are both register direct or register indirect, the function will encode them in the same word.
 * 
 * For two-operand instructions, the function handles source and destination operands separately.
 * For single-operand instructions, only the destination operand is processed. 
 *
 * @param list Pointer to the array of word_data structures representing the machine code.
 * @param command The command_data structure containing the parsed command information.
 * @param error Pointer to an error structure to store any errors encountered during the process.
 * @param IC Pointer to the current instruction counter (IC), which is incremented as words are added.
 * @param line_number The line number of the command in the assembly source file, used for error reporting.
 * @return int Returns 1 if the command was added successfully, and 0 if an error occurred.
 */
int add_command_to_words(word_data **list, command_data command, error *error, int *IC, int line_number);


/**
 * @brief Calculates the number of words required for a given command.
 *
 * This function determines the number of words needed based on the opcode and the types of the source and destination operands.
 * it get the amount of variables from the opcode,
 * if the variables are register direct or register indirect, it will lower the amount of words by 1.
 * and return the amount of words needed + 1;
 *
 * @param command The command data structure containing the opcode and operand types.
 * @return The number of words required for the command.
 */
int get_amount_of_words_from_command(command_data command);


/**
 * @brief Frees memory allocated for a list of word data structures.
 *
 * This function iterates through an array of `word_data` structures and frees the memory allocated for each `symbol`.
 * After freeing all individual symbols, it frees the memory allocated for the array itself.
 *
 * @param words A pointer to the array of `word_data` structures.
 * @param length The number of elements in the `words` array.
 */
void free_word_data_list(word_data *words, int length);

#endif