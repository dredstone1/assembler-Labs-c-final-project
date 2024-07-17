#ifndef MMN14_CONSTS_H
#define MMN14_CONSTS_H

#define MAX_LINE_LENGTH 80

#define BLACK_COLOR "\033[0m"
#define RED_COLOR "\x1b[38;2;247;84;100m"

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
	REGISTER_DIRECT,
	NONE= -1
} variable_type;

typedef struct instruction_data{
	char *label;
	short *numbers;
	int size;
	char *args;
	int is_extern;
} instruction_data;
#endif
