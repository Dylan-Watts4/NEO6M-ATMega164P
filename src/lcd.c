/**
 * @file lcd.c
 * @author Dylan Watts
 * @date 18/1/2021
 * @version 1.0
 * @brief LCD driver for the ATMega164P
*/

#include "lcd.h"

// AVR ATMega164P
// LCD Midas MC42005A6W-BNMLW3.3-V2
// Controller: ST7066U

// LCD pinout
// 1 - VSS - GND
// 2 - VDD - 3.3V
// 3 - NC - No connection
// 4 - RS - Register select
// 5 - R/W - Read/Write
// 6 - E - Enable
// 7 - DB0 - Data bus
// 8 - DB1 - Data bus
// 9 - DB2 - Data bus
// 10 - DB3 - Data bus
// 11 - DB4 - Data bus
// 12 - DB5 - Data bus
// 13 - DB6 - Data bus
// 14 - DB7 - Data bus
// 15 - LED+ - Backlight anode (3.3V)
// 16 - LED- - Backlight cathode

// Function Operation, please refer to the ST7066U datasheet Page 9 Table 1

// PIN DEFINITIONS ** MAKE SURE THESE ARE CORRECT **
#define IO_RS_RW_PORT   DDRB
#define RS_RW_PORT      PORTB
#define E_PIN           PB5
#define RS_PIN          PB6
#define RW_PIN          PB7
#define IO_DB_PORT      DDRC
#define DB_PINS         PORTD

/**
 * @brief Set the respective pins to output mode
 * @warning Ensure that the defined pins are correct before runtime
*/
void initLCD(void) {
    IO_RS_RW_PORT |= (1 << RS_PIN) | (1 << RW_PIN) | (1 << E_PIN);
    IO_DB_PORT = 0xFF;
    // TODO: Any other pins?
}

/**
 * @brief Set RS_PIN and RW_PIN to LCD
 * @param RS_BIT Register select
 * @param RW_BIT Read/Write
 * @warning Ensure that the defined pins are correct before runtime
*/
void setFunction(bool RS_BIT, bool RW_BIT) {
    // Send RS_PIN and RW_PIN to LCD
    RS_RW_PORT &= (RS_BIT << RS_PIN) & (RW_BIT << RW_PIN);
}

/**
 * @brief Set the data bus to the given data
 * @param data Data to be sent to the LCD
 * @warning Ensure that the defined pins are correct before runtime
*/
void setDBData(uint8_t data) {
    DB_PINS = data;
}

/**
 * @brief Send data to the LCD
 * @param RS_BIT Register select
 * @param RW_BIT Read/Write
 * @param data Data to be sent to the LCD
 * @warning Ensure that the defined pins are correct before runtime
 * @note This function is a wrapper for setFunction() and setDBData()
 * @see setFunction()
 * @see setDBData()
*/
void sendData(bool RS_BIT, bool RW_BIT, uint8_t data) {
    // Set the function of the conroller
    setFunction(RS_BIT, RW_BIT);
    // Set the data bus
    setDBData(data);
}

/**
 * @brief Send data to the LCD more specifically the DR
 * @param data Data to be sent to the LCD
 * @warning Ensure that the defined pins are correct before runtime
 * @see sendData()
*/
void writeChar(uint8_t data) {
    sendData(1, 0, data);
}

/**
 * @brief Send a command to the LCD more specifically the IR
 * @param command Command to be sent to the LCD
 * @warning Ensure that the defined pins are correct before runtime
 * @see sendData()
*/
void writeCommand(uint8_t command) {
    sendData(0, 0, command);
}

// HAHAHA THE LCD MAPS WITH ASCII, I AM SAVED
/**
 * @brief Maps the ASCII characters to the LCD character map
 * @warning This map starts from ASCII 44 (',') and ends at ASCII 90 ('Z')
*/
uint8_t lcdCharacterMap[46] = {
    0x2C, 0x2D, 0x2E, 0x2F, 0x30,   // , - . / 0
    0x31, 0x32, 0x33, 0x34, 0x35,   // 1 2 3 4 5
    0x36, 0x37, 0x38, 0x39, 0x3A,   // 6 7 8 9 :
    0x3B, 0x3C, 0x3D, 0x3E, 0x3F,   // ; < = > ?
    0x40, 0x41, 0x42, 0x43, 0x44,   // @ A B C D
    0x45, 0x46, 0x47, 0x48, 0x49,   // E F G H I
    0x4A, 0x4B, 0x4C, 0x4D, 0x4E,   // J K L M N
    0x4F, 0x50, 0x51, 0x52, 0x53,   // O P Q R S
    0x54, 0x55, 0x56, 0x57, 0x58,   // T U V W X
    0x59, 0x5A                      // Y Z
};

/**
 * @brief Write a string to the LCD
 * @param str String to be written to the LCD
 * @warning Ensure that the defined pins are correct before runtime
 * @see lcdCharacterMap
 * @see writeChar()
*/
void writeString(char *str) {
    for (uint8_t i = 0; i < strlen(str); i++) {
        char c = str[i];
        if (c >= 44 && c <= 90) {
            writeChar(lcdCharacterMap[c - 44]);
        } else {
            writeChar(0xFF);
        }
    }
}