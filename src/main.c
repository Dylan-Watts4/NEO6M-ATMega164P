#include <avr/io.h>
#include <util/delay.h>
#include "gps.h"

#define UP_PIN PD0;
#define DOWN_PIN PD1;
#define LEFT_PIN PD2;
#define RIGHT_PIN PD3;

enum Menu {
    MAIN,
    ALARM,
};

int main(void) {
    sei();
    // Make a new menu object
    enum Menu menu = MAIN;

    // Initialize Components
    initGPS();
    initButton();

    // Main loop
    while (1) {
        switch (menu) {
            case MAIN:
                break;
        }

        delay_ms(50);
    }

    return 0;
}

// Assuming PORTD connection
void initButton(void) {
    // Set as input and enable pull-up resistors
    DDRD &= ~((1 << UP_PIN) | (1 << DOWN_PIN) | (1 << LEFT_PIN) | (1 << RIGHT_PIN));
    PORTD |= (1 << UP_PIN) | (1 << DOWN_PIN) | (1 << LEFT_PIN) | (1 << RIGHT_PIN);
    // Enable external interrupt on INT0
    EIMSK |= (1 << INT0);
    // Trigger interrupt on falling edge
    EICRA |= (1 << ISC01);
}

// Button interrut logic
ISR(INT0_vect) {
    // Button logic may need to make menu global
}