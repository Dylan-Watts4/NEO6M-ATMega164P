/**
 * @file uart.c
 * @author Dylan Watts
 * @date 17/1/2024
 * @version 1.0
 * @brief UART driver for the ATMega164P
*/

#include "uart.h"

/**
 * @brief Initialize UART with a given baud rate
 * @details For more information on the ATMega164P datasheet, see page 230
 * @param baud Baud rate of the UART connection
 * @note This function must be called before any UART communication
 * @warning Ensure the baud rate is the same as the receiving device
*/
void initUART(uint32_t baud) {
    // Set baud rate
    UBRR0H = (F_CPU / 16 / baud - 1) >> 8;
    UBRR0L = (F_CPU / 16 / baud - 1);

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    // Set frame format: 8data, 1stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/**
 * @brief Send a single character over UART
 * @details For more information on the ATMega164P datasheet, see page 231
 * @param data Character to send
 * @warning This function will block until the character is sent
*/
void sendUART(char character) {
    // Wait for empty transmit buffer
    while (!(UCSR0A & (1 << UDRE0)));
    // Put data into buffer, sends the data
    UDR0 = character;
}

/**
 * @brief Send a string over UART
 * @param str Pointer to the a null-terminated string
 * @warning This function will block until the string is sent
*/
void sendStringUART(char *str) {
    while (*str) {
        sendUART(*str++);
    }
}


// TODO: Possibly implement a timeout for readUART and readStringUART
/**
 * @brief Read a single character from UART
 * @warning This function will block until a character is received
 * @return Character received
*/
char readUART(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

/**
 * @brief Read a string from UART
 * @param buffer Pointer to the buffer to store the string
 * @param bufferSize Size of the buffer
 * @warning This function will block until a newline or null character is received
*/
void readStringUART(char *buffer, int bufferSize) {
    int i = 0;
    while (i < bufferSize - 1) {
        char c = readUART();
        if (c == '\n' || c == '\0') {
            break;
        }
        buffer[i++] = c;
    }
    buffer[i] = '\0';
}

/**
 * @brief Flush the UART buffer
 * @warning This function will block until the buffer is flushed
*/
void flushUART(void) {
    while (UCSR0A & (1 << RXC0));
}