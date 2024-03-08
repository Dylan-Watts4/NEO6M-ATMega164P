#include "uart.h"

#define BAUD 129 // 9600 baud

void USART_Init(void) {
    UBRR0 = BAUD;
    UCSR0B = 9; 
}

unsigned char USART_Receive(void) {
    // Wait for data to be received -- If running of interrupt, this may not be needed
    while (!(UCSR0A & (1 << RXC)));
    return UDR0;
}

void USART_Flush(void) {
    unsigned char dummy;
    while (UCSR0A & (1 << RXC)) dummy = UDR0;
}

// Function to map the sentence to the global sentence structure
void updateSentence(unsigned char *sentence) {
    // See the strtok function for more information
    char delim = ','; // Delimiter, for the strtok function
    char *token; // Token, for the strtok function, needs to be char* type as size varies
    token = strtok(sentence, delim);

    // Each GLL sentence has a known amount of data seperated by commas, with this in mind
    // we can use a switch statement to map the data to the global sentence structure
    int index = 0;
    while (token != NULL) {
        if (token[3] == 'G' && token[4] == 'L' && token[5] == 'L') {
            switch (index) {
                case 0:
                    strcpy(Sentence.type, token); // strcpy is used to copy the array to another array
                    break;
                case 1:
                    strcpy(Sentence.latitude, token);
                    break;
                case 2:
                    strcpy(Sentence.NS, token);
                    break;
                case 3:
                    strcpy(Sentence.longitude, token);
                    break;
                case 4:
                    strcpy(Sentence.EW, token);
                    break;
                case 5:
                    strcpy(Sentence.time, token);
                    break;
                case 6:
                    strcpy(Sentence.status, token);
                    break;
                case 7:
                    strcpy(Sentence.mode, token);
                    break;}   
        } else break;

        token = strtok(NULL, delim); // Gets the next string seperated by the delimiter
    }
}

// Interrupt Service Routine for USART Receive
ISR(USART_RX_vect) {
    // $ is the start of sentence, while * is the end
    unsigned char data = NULL;
    char sentence[55];
    // int i is used to keep track of the index of the sentence
    int i = NULL;
    do {
        // Get the first character
        data = USART_Receive();
        // If the character is $, reset the index
        if (data == '$') {
            i = 0; // Start of sentence
            sentence = char[55]; // Not sure if the syntax is correct, but reset array
        }
        else if (data == '*') { // End the sentence
            sentence[i] = NULL; // Add a null pointer to sentence
            i = NULL; // Reset the index
            break;
        }
        if (i != NULL) { // While the index is not reset, add the character to the sentence
            sentence[i] = data;
            i++;
        }
    } while (data != NULL);
    updateSentence(sentence); // Update the global sentence
}