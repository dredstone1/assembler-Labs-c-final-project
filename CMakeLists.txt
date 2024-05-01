cmake_minimum_required(VERSION 3.28)
project(mmn14 C)

set(CMAKE_C_STANDARD 11)

add_executable(mmn14
        compiler.h
        compiler.c
        instruction/opcode.c
        instruction/opcode.h
        instruction/word.c
        instruction/word.h
        instruction/addressing_modes.c
        instruction/addressing_modes.h
        instruction/operand_address.c
        instruction/operand_address.h)
