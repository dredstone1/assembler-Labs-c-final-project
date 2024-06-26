cmake_minimum_required(VERSION 3.28)
project(mmn14 C)

set(CMAKE_C_STANDARD 11)

add_executable(mmn14
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
        scripts/assembler/translation/utilities/octal_translator.c
        scripts/assembler/translation/utilities/octal_translator.h
        scripts/data/line_node.c
        scripts/data/line_node.h
        scripts/assembler/translation/symbol/symbol_table.c
        scripts/assembler/translation/symbol/symbol_table.h
        scripts/assembler/translation/code_translation.c
        scripts/assembler/translation/code_translation.h
        scripts/assembler.c
        scripts/assembler.h
        scripts/assembler/translation/word/word_node.c
        scripts/assembler/translation/word/word_node.h
        scripts/assembler/translation/word/words_block.c
        scripts/assembler/translation/word/words_block.h
        scripts/assembler/translation/word/word.c
        scripts/assembler/translation/word/word.h
        scripts/assembler/translation/utilities/line_data.c
        scripts/assembler/translation/utilities/line_data.h
        scripts/assembler/translation/utilities/opcode.c
        scripts/assembler/translation/utilities/opcode.h
        scripts/assembler/translation/utilities/directive.c
        scripts/assembler/translation/utilities/directive.h
)
