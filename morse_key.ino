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

// Message to post
char msg[] = "";

int buzzPin = 5;
int rledPin = 9;
int gledPin = 8;
int ledPin = rledPin;
int morsePin = 7;
boolean keyDown = false;
boolean paused = false;
unsigned long time = 0;

int DIT = 100;          //Duration for dot
int DAH = 3 * DIT;      //Duration for dash
int L_SPACE = 3 * DIT;  //Space between letters
int W_SPACE = 7 * DIT;  //Space between words

String morseCode = "";
String message = "";
char tweet[140];

char LATIN_CHARACTERS[] = {
    // Numbers
    '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9',
    // Letters 
    'a', 'b', 'c', 'd', 'e',
    'f', 'g', 'h', 'i', 'j',
    'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't',
    'u', 'v', 'w', 'x', 'y', 'z',
    // Special
    '.', '?', '@', ' '
};

String MORSE_CHARACTERS[] = {
    // Numbers
    "-----", ".----", "..---", "...--", "....-",
    ".....", "-....", "--...", "---..", "----.",
    // Letters
    ".-", "-...", "-.-.", "-..", ".",
    "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---",
    ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--..",
    // Special
    ".-.-.-", "..--..", ".--.-.", " "
};

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
}

void resetAll()
{

    paused = true;
    Serial.println("reset");

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
    if (morseCode.length() == 0)
        return;
    paused = true;

    // find character from table
    bool decoded = false;
    for (int i = 0; i < 40; i++) {
        if (morseCode == MORSE_CHARACTERS[i]) {
            message += LATIN_CHARACTERS[i];
            decoded = true;
            tone(buzzPin, 800, 50);
            break;
        }
    }

    if (!decoded)
        message += "*";
    morseCode = "";
    Serial.println("\n\r" + message);
    // if (message.length()==130) sendTweet();

    paused = false;
}

void loop()
{
    if (paused)
        return;

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
            } else if (duration < (1.5 * DAH)) {    // DAH detected
                morseCode += "-";
                //DAH=(DAH+duration)/2;
                //DIT=DAH/3;
                Serial.print("-");
            }

            time = millis();

        } else {    // the key was UP before
            unsigned long duration = millis() - time;    //Duration for UP time
            if ((duration > 60000) && (message != "")) {    // reset after 60 seconds inactivity
                resetAll();
            } else if (duration >= (W_SPACE + 50) && (morseCode != "")) {    // start a new word 
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
            } else if ((duration > L_SPACE + 50) && (duration < W_SPACE)) {
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
