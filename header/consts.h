#ifndef MMN14_CONSTS_H
#define MMN14_CONSTS_H

/* define the maximum length of the line */
#define MAX_LINE_LENGTH 83

/* define the maximum length of a symbol and a macro name */
#define MAX_SYMBOL_SIZE 31

/* define the length of the tab */
#define TAB_LENGTH 4

/* define the saved memory for the os */
#define OS_SAVED_MEMORY_CELL 100

/* define the amount of registers */
#define LAST_REGISTER_COUNT 7

/* define the maximum amount of numbers in the data */
#define MAX_VARIABLE_DATA_SIZE 37

/* define the format of the object(".ob") file */
#define LENGTH_OF_WORD_ADDRESS_TO_BE_PRINTED 4
#define LENGTH_OF_WORD_CONTENT_TO_BE_PRINTED 5
#define SPACE_BETWEEN_WORD_ADDRESS_AND_CONTENT_TO_BE_PRINTED 1
#define SPACE_BETWEEN_SUBJECT_IC_DC_TO_BE_PRINTED 1

/* define the format of the entry(".ent") file */
#define LENGTH_OF_WORD_ADDRESS_OF_ENTRY_TO_BE_PRINTED 4
#define SPACE_BETWEEN_SYMBOL_NAME_AND_ADDRESS_OF_ENTRY_TO_BE_PRINTED 1

/* define the format of the external(".ext") file */
#define LENGTH_OF_WORD_ADDRESS_OF_EXTERNAL_TO_BE_PRINTED 4
#define SPACE_BETWEEN_SYMBOL_NAME_AND_ADDRESS_TO_BE_PRINTED 1

/* define the length of the file name ending */
#define FILE_NAME_ENDING_LENGTH 5

/* define the different file endings */
#define SOURCE_FILE_ENDING "as" /* the ending of the source file */
#define SOURCE_FILE_AFTER_POST_ASSEMBLER_ENDING "am" /* the ending of the source file after the post assembler */
#define OBJECT_FILE_ENDING "ob" /* the ending of the object file */
#define ENTRY_FILE_ENDING "ent" /* the ending of the entry file */
#define EXTERNAL_FILE_ENDING "ext" /* the ending of the external file */

/* define the macro commands to be used in the post assembler */
#define MACRO "macr"
#define END_MACRO "endmacr"

/* define the amount of memory cell the cpu supports */
#define MAXIMUM_NUMBER_OF_TOTAL_WORDS 4096


/**
 * @brief An enumeration of the different opcodes.
 */
typedef enum opcode {
	MOV,
	CMP,
	ADD,
	SUB,
	LEA,
	CLR,
	NOT,
	INC,
	DEC,
	JMP,
	BNE,
	RED,
	PRN,
	JSR,
	RTS,
	STOP
} opcode;


/**
 * @brief An enumeration of the different directive types.
 */
typedef enum directive_type {
	DATA,
	STRING,
	ENTRY,
	EXTERN
} directive_type;


/**
 * @brief An enumeration of the different variable types.
 */
typedef enum variable_type {
	IMMEDIATE,
	DIRECT,
	REGISTER_INDIRECT,
	REGISTER_DIRECT
} variable_type;


/**
 * @brief An enumeration of some of the different saved words and their rules
 */
extern const char *opcode_names[][3];
extern const char *operand_names[];

/* An array of the directive type names */
extern const char *directive_type_names[];

#endif
