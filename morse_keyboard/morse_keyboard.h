/*
  Copyright (c) 2017 Tong Hui <tonghuix@member.fsf.org>
  
  Based on TworseKey http://modin.yuri.at/tworsekey/

  TworseKey is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
char decodeMorse(String morse);
void readMorse();

void setup();
void loop();

#endif



