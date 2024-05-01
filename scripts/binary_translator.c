#include "binary_translator.h"

int intBINARY_to_binary(int num){
    int binary = 0, i = 1;

    while (num != 0){
        binary += (num%2) * i;
        num /= 2;
        i *= 10;
    }

    return binary;
}

int size_of_binary(int num){
    int size = 0;

    while (num != 0){
        num /= 10;
        size++;
    }

    return size;
}

int size_of_intBINARY(int num){
    int size = 0;

    while (num != 0){
        num >>= 1;
        size++;
    }

    return size;
}

int array_of_int_to_binaryINT(int list[], int size_of_list){
    int binary = 0, i;

    for (i = 0; i < size_of_list; i++){
        binary <<= size_of_intBINARY(list[i]);
        binary |= list[i];
    }

    return binary;
}
