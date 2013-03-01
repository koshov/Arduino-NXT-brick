#include <Wire.h>

int leftM0 = 11;    // Left motor black wire
int leftM1 = 10;    // Left motor white wire
int rightM0 = 5;    // Right motor black wire
int rightM1 = 6;    // Right motor white wire

void setup()
{
    Wire.begin(4);
    Wire.onReceive(receiveEvent);    // join i2c bus with address #4
    Serial.begin(9600);
    Serial.println("Hello!");    // start serial for output

    pinMode(leftM0, OUTPUT);
    pinMode(leftM1, OUTPUT);
    pinMode(rightM0, OUTPUT);
    pinMode(rightM1, OUTPUT);
}

void loop()
{
    delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany){
    byte left, right;
    while(Wire.available()) // loop through all but the last
    {
        left = Wire.read(); // receive byte as a character
        right = Wire.read();
    }
    processVals(left, right);
}

void processVals(byte left, byte right){
    Serial.println(left);
    boolean direction_left =  bitRead(left, 7);
    Serial.print(direction_left);
    Serial.print(" - ");
    bitClear(left, 7);
    Serial.print(left<<1);
    Serial.print("; ");

    boolean direction_right =  bitRead(right, 7);
    Serial.print(direction_right);
    Serial.print(" - ");
    bitClear(right, 7);
    Serial.print(right<<1);
    Serial.println("; ");
    
    if (direction_left) {    // Move left
        analogWrite(leftM1, 0);
        analogWrite(leftM0, left<<1);
    } else {
        analogWrite(leftM0, 0);
        analogWrite(leftM1, left<<1);
    }
    if (direction_right) {    // Move rigth
        analogWrite(rightM1, 0);
        analogWrite(rightM0, right<<1);
    } else {
        analogWrite(rightM0, 0);
        analogWrite(rightM1, right<<1);
    }
}
