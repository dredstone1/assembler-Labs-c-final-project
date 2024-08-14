#include "../header/consts.h"

const char *opcode_names[][3] = {
		{"mov",  "_123", "0123"},
		{"cmp",  "0123", "0123"},
		{"add",  "_123", "0123"},
		{"sub",  "_123", "0123"},
		{"lea",  "_123", "_1__"},
		{"clr",  "_123", "____"},
		{"not",  "_123", "____"},
		{"inc",  "_123", "____"},
		{"dec",  "_123", "____"},
		{"jmp",  "_12_", "____"},
		{"bne",  "_12_", "____"},
		{"red",  "_123", "____"},
		{"prn",  "0123", "____"},
		{"jsr",  "_12_", "____"},
		{"rts",  "____", "____"},
		{"stop", "____", "____"}
};

const char *directive_type_names[] = {
		"data",
		"string",
		"entry",
		"extern"
};

const char *operand_names[] = {
		"immediate",
		"direct",
		"register direct",
		"register indirect"
};
