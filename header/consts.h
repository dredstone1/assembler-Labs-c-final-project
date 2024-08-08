#ifndef MMN14_CONSTS_H
#define MMN14_CONSTS_H

#define MAX_LINE_LENGTH 83
#define MAX_SYMBOL_SIZE 31
#define MACRO "macr"
#define END_MACRO "endmacr"

typedef enum opcode{
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

typedef enum directive_type{
	DATA,
	STRING,
	ENTRY,
	EXTERN
} directive_type;


typedef enum variable_type{
	IMMEDIATE,
	DIRECT,
	REGISTER_INDIRECT,
	REGISTER_DIRECT
} variable_type;

typedef struct instruction_data{
	short *numbers;
	int size;
	char args[MAX_SYMBOL_SIZE];
	int is_extern;
} instruction_data;

extern const char *opcode_names[][3];
extern const char *operand_names[];
extern const char *instruction_type_names[];

#endif
