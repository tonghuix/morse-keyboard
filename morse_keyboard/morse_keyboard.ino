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

#include "morse_keyboard.h"

const int buzzPin = 5;
const int rledPin = 9;
const int gledPin = 8;
const int ledPin = rledPin;
const int morsePin = 7;

boolean keyDown = false;
boolean paused = false;
boolean capslock = false;

unsigned long time = 0;

void resetAll()
{

    paused = true;
    Serial.println("All Reset..");

    // play earcon
    delay(200);
    tone(buzzPin, 2000, 50);
    delay(100);
    tone(buzzPin, 2000, 50);
    delay(100);
    tone(buzzPin, 2000, 50);
    delay(100);

    // reset strings
    morseCode = "";

    message = "";

    time = millis();
    paused = false;
    return;

    Keyboard.releaseAll();
}


char decodeMorse(String morse)
{
    char tempCharacter = NULL;
    if (morse[0] != '-' && morse[0] != '.')
        return NULL;
    
    paused = true;
    // find character from table
    bool decoded = false;
    for (int i = 0; i < sizeof(LATIN_CHARACTERS); i++) {
        if (morse == MORSE_CHARACTERS[i]) {
            decoded = true;
            if (capslock == true && i < 26)
                tempCharacter = LATIN_CHARACTERS[i] - ('a' - 'A');
            else 
                tempCharacter = LATIN_CHARACTERS[i];
        }
    }

    if (!decoded) {
        tone(buzzPin, 1500, 100);
        tempCharacter = '*';
    }

    Serial.print(tempCharacter);  //debug
    Keyboard.write(tempCharacter);
    message += tempCharacter;
    morse = "";

    Serial.println("\n\r" + message);  //debug

    paused = false;
}

void readMorse()
{
    if (digitalRead(morsePin) == HIGH){    // the morse key is UP
        if (keyDown) {    // the key was DOWN before
            unsigned long duration = millis() - time;
            if (duration < 20)
                return;

            digitalWrite(ledPin, LOW);    // turn LED  OFF
            noTone(buzzPin);    // turn BUZZ OFF
            keyDown = false;

            if (duration < (DIT + 50)) {    // DIT detected
                morseCode += ".";
                Serial.print(".");
                if (morseCode == "...---...")
                    Serial.println("SOS");
            } else if (duration < (1.2 * DAH)) {    // DAH detected
                morseCode += "-";
                //DAH=(DAH+duration)/2;
                //DIT=DAH/3;
                Serial.print("-");
                if (morseCode == "----"){
                    capslock = !capslock;
                    Serial.print(capslock);
                }
            }

            time = millis();

        } else {    // the key was UP before
            unsigned long duration = millis() - time;    //Duration for UP time
            if ((duration > 60000) && (message != "")) {    // reset after 60 seconds inactivity
                resetAll();
            } else if (duration >= (W_SPACE + 150) && (morseCode != ""))
            {    // start a new word 
                decodeMorse(morseCode);
                if (morseCode == "......"){
                    morseCode = "";
                    return;
                } else {
                    message += " ";
                    Keyboard.write(' ');
                    morseCode = "";
                    tone(buzzPin, 2000, 50);    // short feedback beep
                }
            }
        }
    } else {        // the morse key is DOWN
        if (!keyDown) {    // the key was UP before
            unsigned long duration = millis() - time;
            if (duration < 20) {
                return;
            } else if ((duration > L_SPACE + 50) && (duration < W_SPACE + 100)) {
                decodeMorse(morseCode);    // decode last letter
                morseCode = "";
            }
 
            keyDown = true;
            time = millis();

            digitalWrite(ledPin, HIGH);    // turn LED  ON
            tone(buzzPin, 440);    // turn BUZZ ON
        } else {    // the key was already DOWN
            unsigned long duration = millis() - time;
            if (duration > 1000 && duration < 1500) {
                Keyboard.write(KEY_RETURN);
                digitalWrite(ledPin, LOW);    // turn LED  OFF
                noTone(buzzPin);    // turn BUZZ OFF
                keyDown = false;
                Serial.print("\n\r");
            }
        }
    }
}

void setup()
{
    // define the pins
    pinMode(rledPin, OUTPUT);
    pinMode(gledPin, OUTPUT);
    pinMode(buzzPin, OUTPUT);    // output for the buzzer
    pinMode(morsePin, INPUT);    // input from the morse switch 
    digitalWrite(morsePin, HIGH);    // turns on pull-up resistor

    // initialize connections
    Serial.begin(9600);
    Keyboard.begin();
    delay(1000);
    time = millis();
    resetAll();
}

void loop()
{
    if (paused)
        return;

    readMorse();
}




