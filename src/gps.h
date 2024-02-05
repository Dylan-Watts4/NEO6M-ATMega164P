#ifndef GPS_H
#define GPS_H

#include <avr/io.h>
#include <stdint.h>
#include <string.h>

#ifndef F_CPU
#define F_CPU 20e6
#endif

// Structs
typedef struct {
    char sentence[6];   // $GPGLL
    char latitude[9];   // ddmm.mmmm
    char north_south;   // N or S
    char longitude[10]; // dddmm.mmmm
    char east_west;     // E or W
    char utc_time[10];  // hhmmss.sss
    char status;        // A (valid) or V (not valid)
    char checksum[4]    // Mode and checksum combined, POSSIBLE OVERFLOW!!
} GLLSentence;

// Functions

#endif