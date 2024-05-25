cmake_minimum_required(VERSION 3.28)
project(mmn14 C)

set(CMAKE_C_STANDARD 11)

add_executable(mmn14
        scripts/compiler.h
        scripts/compiler.c
        scripts/binary_translator.c
        scripts/binary_translator.h
        scripts/assembler/post_formating/post_formating.c
        scripts/assembler/post_formating/post_formating.h
        scripts/systems/file.c
        scripts/systems/file.h
        scripts/systems/error.c
        scripts/systems/error.h
        scripts/data/boolean.c
        scripts/data/boolean.h
        scripts/assembler/post_formating/macro.c
        scripts/assembler/post_formating/macro.h
        scripts/data/line.c
        scripts/data/line.h
        scripts/octal_translator.c
        scripts/octal_translator.h
        scripts/data/line_node.c
        scripts/data/line_node.h
        scripts/assembler/symbol/symbol_table.c
        scripts/assembler/symbol/symbol_table.h
        scripts/assembler/symbol/symbol_node.c
        scripts/assembler/symbol/symbol_node.h
        scripts/assembler/first_pass/first_pass.c
        scripts/assembler/first_pass/first_pass.h
        scripts/assembler/assembler.c
        scripts/assembler/assembler.h
        scripts/assembler/first_pass/word_node.c
        scripts/assembler/first_pass/word_node.h
        scripts/assembler/first_pass/words_block.c
        scripts/assembler/first_pass/words_block.h
        scripts/assembler/first_pass/word.c
        scripts/assembler/first_pass/word.h
        scripts/assembler/line_data.c
        scripts/assembler/line_data.h
        scripts/assembler/symbol/symbol.c
        scripts/assembler/symbol/symbol.h
        scripts/assembler/opcode.c
        scripts/assembler/opcode.h
        scripts/assembler/directive.c
        scripts/assembler/directive.h
)
