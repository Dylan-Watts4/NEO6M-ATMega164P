#ifndef LCD_H
#define LCD_H

#include <avr/io.h>
#include <stdio.h>
#include <string.h>

// Functions
void initLCD(void);
void setFunction(bool RS_BIT, bool RW_BIT);
void setDBData(uint8_t data);
void sendData(bool RS_BIT, bool RW_BIT, uint8_t data);
void writeChar(uint8_t data);
void writeCommand(uint8_t command);
void writeString(char *str);

#endif