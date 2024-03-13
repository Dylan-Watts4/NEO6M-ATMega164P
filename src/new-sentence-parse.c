# include <string.h>

// These functions require the use of the strtok function, which is part of the string.h library
// Please see the strtok and strncmp functions for more information

// --------------------------------------------------------------------------------------------
// -- Parsing Algorithm -- //
// 1. Get the first character
// 2. If the character is $, reset the index
// 3. If the character is *, end the sentence
// 4. If the index is not reset, add the character to the sentence
// 5. Repeat until the character is NULL or '\0'
// 6. Do something with the sentence
ISR(USART_RX_vect) {
    unsigned char type = "$GPGLL"; // The type of sentence to receive
    unsigned char sentence[55]; // Buffer to store the sentence
    unsigned char data = UDR0; // Get the first character
    int index = -1; // Index of the sentence
    do {
        if (data == '$') {
            // Reset the sentence index
            index = 0;
            sentence = char[55]; // Reset the sentence
        }
        else if (data == '*') {
            // End the sentence with null pointer
            sentence[index] = NULL;
            // Reset the sentence index
            index = -1;
            // Check weather the sentence is the correct type
            if (strncmp(sentence, type, 6) == 0) {
                // Handle the data
                splitData(sentence);
                break; // Break the loop since we have the correct sentence
            }
        }
        if (index != -1) {
            // Add the character to the sentence
            sentence[index] = data;
            index++;
        }
        data = UDR0;
    } while (data != NULL);
}

// - Splitting the data - //
// 1. Split the sentence into tokens
// 2. Do something with the token
// 3. Increment the index, the index is used to keep track of the data as we know the structure of the sentence
// 4. Repeat until the token is NULL
void splitData(unsigned char* sentence) {
    // Split the sentece into tokens
    char delim = ','; // Delimiter
    char *token; // Token (Holds the data between the delimiters)
    token = strtok(sentence, delim);
    int index = 0;
    while (token != NULL) {
        // Do something with the token
        token = strtok(NULL, delim);
        // TODO: Print the data or store it??

        // Increment the index
        index++;
    }
}
// --------------------------------------------------------------------------------------------