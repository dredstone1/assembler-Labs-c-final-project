cmake_minimum_required(VERSION 3.28)
project(mmn14 C)

set(CMAKE_C_STANDARD 11)

add_executable(mmn14
        scripts/post_formating.c
        header/post_formating.h
        scripts/file.c
        header/file.h
        scripts/error.c
        header/error.h
        scripts/macro.c
        header/macro.h
        scripts/line_node.c
        header/line_node.h
        scripts/symbol_table.c
        header/symbol_table.h
        scripts/assembler.c
        scripts/word_conversion.c
        header/word_conversion.h
        scripts/line_data.c
        header/line_data.h
        scripts/first_pass.c
        header/first_pass.h
        scripts/utilities.c
        header/utilities.h
        scripts/second_pass.c
        header/second_pass.h
        header/consts.h
)
