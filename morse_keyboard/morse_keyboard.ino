/*
  Copyright (c) 2017 Tong Hui
  
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
}


void decodeMorse()
{
    if (morseCode[0] != '-' && morseCode[0] != '.')
        return;
    paused = true;

    // find character from table
    bool decoded = false;
    for (int i = 0; i < sizeof(LATIN_CHARACTERS); i++) {
        if (morseCode == MORSE_CHARACTERS[i]) {
            Serial.print("CAP= ");
            Serial.println(capslock);
            if (capslock == true && i < 26)
            {
                char tempCharacter = LATIN_CHARACTERS[i] - ('a' - 'A');
                Serial.print(tempCharacter);
                message += tempCharacter;
            }
            else 
            {            
                Serial.print(LATIN_CHARACTERS[i]);
                message += LATIN_CHARACTERS[i];
            }
            decoded = true;
            tone(buzzPin, 800, 50);
        }
    }

    if (!decoded) {
        Serial.println("*");
        message += "*";
    }
    
    morseCode = "";
    Serial.println("\n\r" + message);

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
                if (morseCode == "........")
                    resetAll();
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
            } else if (duration >= (W_SPACE + 150) && (morseCode != "")) {    // start a new word 
                decodeMorse();
                if (message != "")    message += " ";
                tone(buzzPin, 2000, 50);    // short feedback beep
            }
        }
    } else {        // the morse key is DOWN
        if (!keyDown) {    // the key was UP before
            unsigned long duration = millis() - time;
            if (duration < 20) {
                return;
            } else if ((duration > L_SPACE + 50) && (duration < W_SPACE + 100)) {
                decodeMorse();    // decode last letter
            }

            keyDown = true;
            time = millis();

            digitalWrite(ledPin, HIGH);    // turn LED  ON
            tone(buzzPin, 440);    // turn BUZZ ON
        } else {    // the key was already DOWN
            unsigned long duration = millis() - time;
            if (duration > 1000) {

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


