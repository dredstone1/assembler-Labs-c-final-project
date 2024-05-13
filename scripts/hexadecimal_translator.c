#include "hexadecimal_translator.h"

/* Function to translate an integer into octal format */
int translateToOctal(int number) {
    int result = 0;

    // Convert the decimal number to octal
    while (number != 0) {
        result = (number % 8) + result * 10;
        number /= 8;
    }

    // Return the octal representation of the number
    return result;
}

/* Main function */
int main() {
    // Pre-defined number
    int number = 123;

    // Call the function to translate the number into octal format
    int octalValue = translateToOctal(number);

    // Return the octal value
    return octalValue;
}