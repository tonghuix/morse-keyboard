#ifndef MAIN_h
#define MAIN_h

#include "Arduino.h"

int DIT = 100;          //Duration for dot
int DAH = 3 * DIT;      //Duration for dash
int L_SPACE = 3 * DIT;  //Space between letters
int W_SPACE = 7 * DIT;  //Space between words

String morseCode = "";
String message = "";

const char LATIN_CHARACTERS[] = {
    // Letters 
    'a', 'b', 'c', 'd', 'e',
    'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z',
    // Numbers
    '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9',
    // Special
    '.', '?', '@', ',', ':',
    '`', '-', '/', '(', ')',
    '"', '=', '+', ';', '_',
    '!', '$', '^', '&', '~',
    '#', '%', 
    // Function Keys
    KEY_BACKSPACE, KEY_ESC
};

const char * const MORSE_CHARACTERS[] = {
    // Letters
    ".-", "-...", "-.-.", "-..", ".",
    "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---",
    ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..",
    // Numbers
    "-----", ".----", "..---", "...--", "....-",
    ".....", "-....", "--...", "---..", "----.",
    // Special
    ".-.-.-", "..--..", ".--.-.", "--..--",
    "---...", ".----.", "-....-", "-..-.",
    "-.--.", "-.--.-", ".-..-.", "-...-",
    ".-.-.", "-.-.-.", "..--.-", "-.-.--",
    "...-..-", "-.-.-", ".-...", ".-.-..",
    "...-..", "...-...", 
    // Function Keys
    "......", "........"
    
};

void resetAll();
void decodeMorse();
void readMorse();

void setup();
void loop();

#endif



