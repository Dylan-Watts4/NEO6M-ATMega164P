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

// PIN DEFINITIONS ** MAKE SURE THESE ARE CORRECT **
// These definitions are to ensure that limited changes to the code need to be made depending on the pinout
#define IO_RS_RW_PORT   DDRB
#define RS_RW_PORT      PORTB
#define E_PIN           PB5
#define RS_PIN          PB6
#define RW_PIN          PB7
#define IO_DB_PORT      DDRD
#define DB_PINS         PORTD

/**
 * @brief Set the respective pins to output mode
 * @warning Ensure that the defined pins are correct before runtime
*/
void initLCD(void) {
    // Set the respective pins to output mode, refer to definitions
    IO_RS_RW_PORT |= (1 << RS_PIN) | (1 << RW_PIN) | (1 << E_PIN);
    IO_DB_PORT = 0xFF;
    // TODO: Any other pins?
    // Entry mode set may need assignment, further tests needed
    // Refer to the ST7066U datasheet page 18 for more information
}

/**
 * @brief Toggle the enable pin
 * @warning Ensure that the defined pins are correct before runtime
*/
void toggleEnable(void) {
    // Toggle the enable pin to 1
    RS_RW_PORT |= (1 << E_PIN);
    // Delay for 1us as per the ST7066U datasheet
    _delay_us(1);
    // Toggle the enable pin to 0
    RS_RW_PORT &= ~(1 << E_PIN);
}

/**
 * @brief Set RS_PIN and RW_PIN to LCD
 * @param RS_BIT Register select
 * @param RW_BIT Read/Write
 * @warning Ensure that the defined pins are correct before runtime
*/
void setFunction(bool RS_BIT, bool RW_BIT) {
    // Clear the RS_PIN and RW_PIN
    RS_RW_PORT &= ~((1 << RS_PIN) | (1 << RW_PIN));
    // More information on the ST7066U datasheet Page 9 Table 1
    // Send RS_PIN and RW_PIN to LCD
    RS_RW_PORT |= ((RS_BIT << RS_PIN) | (RW_BIT << RW_PIN));
}

/**
 * @brief Set the data bus to the given data
 * @param data Data to be sent to the LCD
 * @warning Ensure that the defined pins are correct before runtime
*/
void setDBData(uint8_t data) {
    // The data bus pins carry the data which will go to pins DB0-DB7 on the LCD
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
    // Toggle the enable pin
    toggleEnable();
}

/**
 * @brief Send data to the LCD more specifically the DR
 * @param data Data to be sent to the LCD
 * @warning Ensure that the defined pins are correct before runtime
 * @see sendData()
*/
void writeChar(uint8_t data) {
    // Refer to the ST7066U datasheet Page 9 Table 1 and sendData() for more information
    sendData(1, 0, data);
}

/**
 * @brief Send a command to the LCD more specifically the IR
 * @param command Command to be sent to the LCD
 * @warning Ensure that the defined pins are correct before runtime
 * @see sendData()
*/
void writeCommand(uint8_t command) {
    // Refer to the ST7066U datasheet Page 9 Table 1 and sendData() for more information
    sendData(0, 0, command);
}


// HAHAHA THE LCD MAPS WITH ASCII, I AM SAVED
/**
 * @brief Maps the ASCII characters to the LCD character map
 * @warning This map starts from ASCII 44 (',') and ends at ASCII 90 ('Z')
*/
/*
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
*/

/**
 * @brief Write a string to the LCD
 * @param str String to be written to the LCD
 * @warning Ensure that the defined pins are correct before runtime
 * @see lcdCharacterMap
 * @see writeChar()
*/
void writeString(char *str) {
    // Iterate through the string
    for (uint8_t i = 0; i < strlen(str); i++) {
        // Store character in local memory (c)
        char c = str[i];
        // Refer to the lcdCharacterMap array, begins at ASCII 44 (',') and ends at ASCII 90 ('Z')
        // If further information needed, search ASCII table and verify the characters that will be used
        if (c >= 44 && c <= 90) {
            // Write the character to the LCD, but given that lcdCharacterMap starts at ASCII 44, we need to subtract 44 from the character
            // writeChar(lcdCharacterMap[c - 44]);
            // As the LCD maps with ASCII, we can just write the character to the LCD
            writeChar(c);
        } else {
            // If the character is not in the lcdCharacterMap, write 0xFF to the LCD
            // This is to ensure that the LCD does not display any random characters
            // Refer to the LCD datasheet character map for more information
            writeChar(0x20);
        }
    }
}

/**
 * @brief Clear the LCD screen
 * @warning Ensure that the defined pins are correct before runtime
 * @see setFunction()
 * @see writeCommand()
*/
void clearLCD(void) {
    // Remember that the writeCommand() function already sets the RS_PIN to 0 and the RW_PIN to 0
    // See writeCommand() for more information
    // Clear the LCD screen, refer to the ST7066U datasheet page 17 and 18 for more information
    writeCommand(0x01);
    // Set the cursor to the home position, refer to the ST7066U datasheet page 17 for more information
    returnHome();
}

/**
 * @brief Turn the display on or off
 * @param toggle Toggle the display on or off
 * @warning Ensure that the defined pins are correct before runtime
 * @see writeCommand()
*/
void displayON_OFF(bool toggle) {
    if (toggle) {
        // Turn the display on, refer to the ST7066U datasheet page 17 and 19 for more information
        // May need ammending depending on the testing
        writeCommand(0x0C);
    } else {
        // Turn the display off, refer to the ST7066U datasheet page 17 and 19 for more information
        writeCommand(0x08);
    }
}

/**
 * @brief Check whether the LCD is busy
 * @return Whether the LCD is busy or not
 * @warning Ensure that the defined pins are correct before runtime
 * @see setFunction()
 * @see toggleEnable()
*/
bool isBusy(void) {
    // Set the pins connected to the data bus to input mode
    IO_DB_PORT = 0x00;
    // Set the function of the controller to check the busy flag
    // Refer to the ST7066U datasheet page 9 table 1 for more information
    setFunction(0, 1);
    toggleEnable();
    // Refer to the ST7066U datasheet page 21 for more information
    // Shift the data bus to the right by 7 bits and check the LSB (least significant bit)
    // If the LSB is 1, the LCD is busy
    if ((DB_PINS >> 7) & 0x01) {
        // Set the pins connected to the data bus to output mode
        setFunction(1, 0);
        return true;
    } else {
        // Set the pins connected to the data bus to output mode
        setFunction(1, 0);
        return false;
    }
}

/**
 * @brief Shift the cursor to the left
 * @warning Ensure that the defined pins are correct before runtime
 * @see writeCommand()
*/
void shiftCursorLeft(void) {
    uint8_t command = 0x10;
    // Refer to the ST7066U datasheet page 19 for more information
    writeCommand(command);
}

/**
 * @brief Shift the cursor to the right
 * @warning Ensure that the defined pins are correct before runtime
 * @see writeCommand()
*/
void shiftCursorRight(void) {
    uint8_t command = 0x14;
    // Refer to the ST7066U datasheet page 19 for more information
    writeCommand(command);
}

/**
 * @brief Return cursor to home position
 * @warning Ensure that the defined pins are correct before runtime
 * @see writeCommand()
*/
void returnHome(void) {
    uint8_t command = 0x02;
    // Refer to the ST7066U datasheet page 18 for more information
    writeCommand(command);
}

/**
 * @brief Shift the cursor to the second line
 * @warning Ensure that the defined pins are correct before runtime
 * @see returnHome()
 * @see shiftCursorRight()
*/
void shiftCursorToNewLine(void) {
    returnHome();
    // The cursor moves to the second line after 40 characters
    // Refer to the ST7066U datasheet page 19 for more information
    for (uint8_t i = 0; i < 40; i++) {
        shiftCursorRight();
    }
}