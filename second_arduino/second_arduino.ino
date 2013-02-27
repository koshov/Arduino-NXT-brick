#include <Wire.h>

int leftM0 = 11;    // Left motor black wire
int leftM1 = 10;    // Left motor white wire
int rightM0 = 5;    // Right motor black wire
int rightM1 = 6;    // Right motor white wire
int left = 0;
int right = 0;

void setup()
{
    Wire.begin(4);
    Wire.onReceive(receiveEvent);    // join i2c bus with address #4
    pinMode(leftM0, OUTPUT);
    pinMode(leftM1, OUTPUT);
    pinMode(rightM0, OUTPUT);
    pinMode(rightM1, OUTPUT);
    Serial.begin(9600);           // start serial for output
}

void loop()
{
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
    while(1 < Wire.available()) // loop through all but the last
    {
        left = Wire.read(); // receive byte as a character
        Serial.print("The left one is ");
        Serial.println(left);
        right = Wire.read();
        Serial.print("The right one is ");
        Serial.println(right);
        processVals(right, left);
     }       
}

void processVals(int right, int left){
    int direction_right = bitRead(right, 7);
    int direction_left = bitRead(left, 7);
    if (direction_right) {    // Move forward
        analogWrite(leftM1, 0);
        analogWrite(rightM1, 0);
        analogWrite(leftM0, right<<1);
        analogWrite(rightM0, right<<1);
    } else {
        analogWrite(leftM0, 0);
        analogWrite(rightM0, 0);
        analogWrite(leftM1, left<<1);
        analogWrite(rightM1, left<<1);
    }
}
