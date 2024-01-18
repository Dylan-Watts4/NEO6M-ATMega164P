#include "lcd.h"

// AVR ATMega164P
// LCD Midas MC42005A6W-BNMLW3.3-V2

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

// Controller: ST7066U

// Function Operation, please refer to the ST7066U datasheet Page 9 Table 1

// PIN DEFINITIONS ** MAKE SURE THESE ARE CORRECT **
#define IO_RS_RW_PORT   DDRB
#define RS_RW_PORT      PORTB
#define RS_PIN          PB6
#define RW_PIN          PB7
#define IO_DB_PORT      DDRC
#define DB_PINS         PORTD

/**
 * @brief Set the respective pins to output mode
 * @warning Ensure that the defined pins are correct before runtime
*/
void initLCD(void) {
    IO_RS_RW_PORT |= (1 << RS_PIN) | (1 << RW_PIN);
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
*/
void sendData(bool RS_BIT, bool RW_BIT, uint8_t data) {
    // Set the function of the conroller
    setFunction(RS_BIT, RW_BIT);
    // Set the data bus
    setDBData(data);
}