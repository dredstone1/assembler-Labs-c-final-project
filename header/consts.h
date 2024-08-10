#ifndef MMN14_CONSTS_H
#define MMN14_CONSTS_H

#define MAX_LINE_LENGTH 83
#define MAX_SYMBOL_SIZE 31
#define FILE_NAME_ENDING_LENGTH 5
#define IC_START_COUNT 100
#define MACRO "macr"
#define END_MACRO "endmacr"
#define SOURCE_FILE_ENDING "as"
#define SOURCE_FILE_AFTER_POST_ASSEMBLER_ENDING "am"
#define OBJECT_FILE_ENDING "ob"
#define ENTRY_FILE_ENDING "ent"
#define EXTERNAL_FILE_ENDING "ext"
#define FIRST_REGISTER_COUNT 0
#define LAST_REGISTER_COUNT 7
#define MAX_VARIABLE_DATA_SIZE 76


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


typedef enum directive_type {
	DATA,
	STRING,
	ENTRY,
	EXTERN
} directive_type;


typedef enum variable_type {
	IMMEDIATE,
	DIRECT,
	REGISTER_INDIRECT,
	REGISTER_DIRECT
} variable_type;


extern const char *opcode_names[][3];
extern const char *operand_names[];
extern const char *directive_type_names[];

#endif
