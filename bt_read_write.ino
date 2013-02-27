#include <Wire.h>

int leftM0 = 11;    // Left motor black wire
int leftM1 = 10;    // Left motor white wire
int rightM0 = 5;    // Right motor black wire
int rightM1 = 6;    // Right motor white wire

void setup() {
    Wire.begin();
    Serial.begin(9600); 
    pinMode(leftM0, OUTPUT);
    pinMode(leftM1, OUTPUT);
    pinMode(rightM0, OUTPUT);
    pinMode(rightM1, OUTPUT);
    Serial.println("Hello!");
}

void loop() {
    if (Serial.available()){
        int ctrlVals[5];    // Array to store control values
        int i = 0;          // Counter for control package validation
        delay(5);           // Timeout for serial buffer consistency
        while (Serial.available() && i<5){
            ctrlVals[i] = Serial.read();
            i++;
        }
        if (i==5) {         // Control package is fine
            for (int i=0; i<5; i++){
                Serial.print(ctrlVals[i]);
                Serial.print('\t');
            }
            Serial.println();
            processVals(ctrlVals);
            Wire.beginTransmission(4);
            Wire.write(ctrlVals[0]);
            Wire.write(ctrlVals[1]);
            Wire.endTransmission();
        } else if (i>5){
            Serial.println("Control signal larger than 5 bytes");
        } else {
            Serial.println("Control signal less than 5 bytes");
        }
    }
}

void processVals(int ctrlVals[5]){
    if (ctrlVals[2] > ctrlVals[3]) {    // Move forward
        analogWrite(leftM1, 0);
        analogWrite(rightM1, 0);
        analogWrite(leftM0, ctrlVals[2]);
        analogWrite(rightM0, ctrlVals[2]);
    } else {
        analogWrite(leftM0, 0);
        analogWrite(rightM0, 0);
        analogWrite(leftM1, ctrlVals[3]);
        analogWrite(rightM1, ctrlVals[3]);
    }
}
