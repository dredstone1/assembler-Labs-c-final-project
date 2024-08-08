cmake_minimum_required(VERSION 3.28)
project(mmn14 C)

set(CMAKE_C_STANDARD 11)

add_executable(mmn14
        scripts/post_formating.c
        scripts/file.c
        scripts/error.c
        scripts/symbol_table.c
        scripts/assembler.c
        scripts/word_conversion.c
        scripts/line_data.c
        scripts/first_pass.c
        scripts/utilities.c
        scripts/second_pass.c
        header/post_formating.h
        header/file.h
        header/error.h
        header/macro.h
        header/symbol_table.h
        header/word_conversion.h
        header/line_data.h
        header/first_pass.h
        header/utilities.h
        header/second_pass.h
        header/consts.h
        scripts/consts.c
)
