#ifndef UART_H
#define UART_H

#ifndef F_CPU
#define F_CPU 20e6
#endif

#include <string.h>

// Might need to change the sizes of the parts
// This structure can be accessed globally through the name 'Sentence'
struct GLLSentence {
    char type[8];
    char latitude[12];
    char NS[2];
    char longitude[12];
    char EW[2];
    char time[12];
    char status[2];
    char mode[2];
} Sentence;

// Only public functions you need to use
void USART_Init(void);
void USART_Flush(void);

#endif