#include "hexadecimal_translator.h"

int int_to_octal(int num) {
    int answer = 0, y = 1;

    while (num != 0) {
        answer += (num % 8) * y;
        num = num / 8;
        y *= 10;
    }

    return answer;
}