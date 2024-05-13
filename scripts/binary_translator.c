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



// add Roee
/* Function to translate a word into binary format */
void translateToBinary(char *word) {
    // Calculate the length of the word
    int length = strlen(word);

    // Print message indicating the start of binary translation
    printf("Binary translation: ");

    // Iterate through each character in the word
    for (int i = 0; i < length; i++) {
        // Iterate through each bit in the character
        for (int j = 7; j >= 0; j--) {
            // Print the value of the j-th bit of the ASCII value of the character
            printf("%d", (word[i] >> j) & 1);
        }
        // Separate each byte with a space
        printf(" ");
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

    // Call the function to translate the word into binary format
    translateToBinary(word);

    // Return 0 to indicate successful completion of the program
    return 0;
}