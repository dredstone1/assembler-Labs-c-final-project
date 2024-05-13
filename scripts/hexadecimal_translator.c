//
// Created by mayan on 13/05/2024.
//

#include "hexadecimal_translator.h"
#include "hexadecimal_translator.h"
#define
/* Function to translate a word into hexadecimal format */
void translateToHex(char *word) {
    // Calculate the length of the word
    int length = strlen(word);

    // Print message indicating the start of hexadecimal translation
    printf("Hexadecimal translation: ");

    // Iterate through each character in the word
    for (int i = 0; i < length; i++) {
        // Print the hexadecimal representation of the ASCII value of the character
        printf("%X ", (int)word[i]);
    }

    // Print a newline character to separate output
    printf("\n");
}

/* Main function */
int main() {
    // Define a character array to store the word
    char word[100];

    // Prompt the user to enter a word
    printf("Enter a word: ");

    // Read the word entered by the user
    scanf("%s", word);

    // Call the function to translate the word into hexadecimal format
    translateToHex(word);

    // Return 0 to indicate successful completion of the program
    return 0;
}