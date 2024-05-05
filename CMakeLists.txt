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
        scripts/instruction/addressing_modes.c
        scripts/instruction/addressing_modes.h
        scripts/instruction/operand_address.c
        scripts/instruction/operand_address.h
        scripts/binary_translator.c
        scripts/binary_translator.h
        scripts/formating/formating.c
        scripts/formating/formating.h
        scripts/file.c
        scripts/file.h
        scripts/error.c
        scripts/error.h
        scripts/boolean.c
        scripts/boolean.h
        scripts/formating/line_type.c
        scripts/formating/line_type.h)
