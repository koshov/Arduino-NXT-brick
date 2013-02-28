#include <Wire.h>

int frontM0 = 11;    // Left motor black wire
int frontM1 = 10;    // Left motor white wire
int backM0 = 5;    // Right motor black wire
int backM1 = 6;    // Right motor white wire

void setup() {
    Wire.begin();
    Serial.begin(9600); 
    pinMode(frontM0, OUTPUT);
    pinMode(frontM1, OUTPUT);
    pinMode(backM0, OUTPUT);
    pinMode(backM1, OUTPUT);
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
                int direction_current = bitRead(ctrlVals[i], 7);
                Serial.print(ctrlVals[i]);
                Serial.print(' - ');
                Serial.print(direction_current);
                Serial.print('; ');
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
    int direction_back = bitRead(ctrlVals[2], 7);
    int direction_front = bitRead(ctrlVals[3], 7);
    if (direction_front) {    // Move forward
        analogWrite(frontM1, 0);
        analogWrite(frontM0, ctrlVals[3]<<1);
    } else {
        analogWrite(frontM0, 0);
        analogWrite(frontM1, ctrlVals[3]<<1);
    }
    if (direction_back) {    // Move forward
        analogWrite(backM0, 0);
        analogWrite(backM1, ctrlVals[2]<<1);
    } else {
        analogWrite(backM1, 0);
        analogWrite(backM0, ctrlVals[2]<<1);
    }
}
