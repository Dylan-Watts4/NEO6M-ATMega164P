/**
 * @file gps.c
 * @author Dylan Watts
 * @date 17/1/2024
 * @version 1.0
 * @brief GPS driver for the ATMega164P
*/

#include "gps.h"
#include "uart.h"

/**
 * @brief Initialize UART with a baud rate of 9600
*/
void initGPS(void) {
    initUART((uint32_t) 9600);
    // Set the protocol to NMEA, and the baud rate to 9600
    // Refer to the NMEA reference manual page 2-3 for more information
    sendStringUART("$PSRF100,1,9600,8,1,0*0C\0");
}

/**
 * @brief Read a GLL sentence from the GPS
 * @return GLLSentence struct containing the sentence data
*/
GLLSentence readGLL(void) {
    GLLSentence sentence;
    char buffer[50];
    uint8_t counter = 0;
    do {
        readStringUART(buffer, 50);
        counter++;
        if (counter > 30) break;        // Timeout after 15 attempts
    } while (buffer[0] != '$' || buffer[1] != 'G' || buffer[2] != 'P' || buffer[3] != 'G' || buffer[4] != 'L' || buffer[5] != 'L');
    if (counter > 15) {
        sentence.status = 'V';
        return sentence;
    }
    // Flush the UART buffer to ensure the next sentence is read correctly
    flushUART();
    // Tokenize the ',' character to seperate the sentence into its components
    char *token = strtok(buffer, ',');
    uint8_t tokenIndex = 0;
    while (token != NULL) {
        switch (tokenIndex) {
            case 0:
                strcpy(sentence.sentence, token);
                break;
            case 1:
                strcpy(sentence.latitude, token);
                break;
            case 2:
                sentence.north_south = token[0];
                break;
            case 3:
                strcpy(sentence.longitude, token);
                break;
            case 4:
                sentence.east_west = token[0];
                break;
            case 5:
                strcpy(sentence.utc_time, token);
                break;
            case 6:
                sentence.status = token[0];
                break;
            case 7:
                strcpy(sentence.checksum, token);
                break;
        }
        token = strtok(NULL, ',');
        tokenIndex++;
        return sentence;
    }
}

char *makeSentenceString(GLLSentence sentence) {
    char buffer[50];
    sprintf(buffer, "Lat: %s %c, Long: %s %c, Time: %s\0", sentence.latitude, sentence.north_south, sentence.longitude, sentence.east_west, sentence.utc_time);
    return buffer;
}

// A valid GLL XORs every character between the $ and the ending *
bool validGLL(GLLSentence sentence) {
    // TODO Implement checksum validation
    // Recreate the buffer, not the most efficient method but who's gonna stop me
    char buffer[50] = "";
    // Required to splice an array, as sentence includes $
    char tempSentence[6];
    // Need to find the memory address of sentence and define the length to be copied
    memcpy(tempSentence, &sentence.sentence, 5 * sizeof(char));
    strcat(buffer, tempSentence); strcat(buffer, ',');
    // The checksum also includes the commas!
    strcat(buffer, sentence.latitude); strcat(buffer, ',');
    strcat(buffer, north_south); strcat(buffer, ',');
    strcat(buffer, longitude); strcat(buffer, ',');
    strcat(buffer, east_west); strcat(buffer, ',');
    strcat(buffer, utc_time); strcat(buffer, ',');
    strcat(buffer, status); strcat(buffer, ',');
    strcat(buffer, checksum[0]); strcat(buffer, '\0');
    
    // The sentence has been made, now it is time to XOR
    uint8_t sum = 0;
    for (int i = 0; i < sizeof(buffer) / sizeof(char); i++) {
        if (buffer[i] == '\0') break;
        sum ^= buffer[i];
    }
    char sumHex[3];
    sprintf(sumHex, "%02x", sum);
    if (sumHex[0] == sentence.checksum[2] && sumHex[1] == sentence.checksum[3])
        return true;
    return false;
}