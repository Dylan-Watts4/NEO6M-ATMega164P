#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <stdint.h>

#ifndef F_CPU
#define F_CPU 20e6
#endif

// Functions
void initUART(uint32_t baudrate);
void sendUART(char character);
void sendStringUART(char *string);
char receiveUART(void);
char* receiveStringUART(void);
void flushUART(void);

#endif