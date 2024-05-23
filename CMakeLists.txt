cmake_minimum_required(VERSION 3.28)
project(mmn14 C)

set(CMAKE_C_STANDARD 11)

add_executable(mmn14
        scripts/compiler.h
        scripts/compiler.c
        scripts/instruction/opcode.c
        scripts/instruction/opcode.h
        scripts/instruction/word.c
        scripts/instruction/word.h
        scripts/binary_translator.c
        scripts/binary_translator.h
        scripts/formating/formating.c
        scripts/formating/formating.h
        scripts/systems/file.c
        scripts/systems/file.h
        scripts/systems/error.c
        scripts/systems/error.h
        scripts/data/boolean.c
        scripts/data/boolean.h
        scripts/data/tag.c
        scripts/data/tag.h
        scripts/formating/macro.c
        scripts/formating/macro.h
        scripts/formating/line.c
        scripts/formating/line.h
        scripts/formating/directive_line_type.c
        scripts/formating/directive_line_type.h
        scripts/octal_translator.c
        scripts/octal_translator.h
        scripts/formating/operation_line_type.c
        scripts/formating/operation_line_type.h
        scripts/formating/line_node.c
        scripts/formating/line_node.h
        scripts/instruction/instruction.c
        scripts/instruction/instruction.h
        scripts/instruction/instruction_node.c
        scripts/instruction/instruction_node.h
        scripts/instruction/symbol_table.c
        scripts/instruction/symbol_table.h
        scripts/instruction/symbol_node.c
        scripts/instruction/symbol_node.h
)
