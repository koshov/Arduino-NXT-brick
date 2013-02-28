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
    Serial.begin(9600);
    Serial.println("Hello!");    // start serial for output
}

void loop()
{
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
    while(Wire.available()) // loop through all but the last
    {
        left = Wire.read(); // receive byte as a character
        right = Wire.read();
        processVals(right, left);
     }
}

void processVals(int right, int left){
    boolean direction_left = bitRead(left, 7);
    Serial.print("The left one is ");
    Serial.print(left);
    Serial.print(". And the direction is ");
    Serial.println(direction_left);
    boolean direction_right = bitRead(right, 7);
    Serial.print("The right one is ");
    Serial.println(right);
    Serial.print(". And the direction is ");
    Serial.println(direction_right);
    
    if (direction_right) {    // Move forward
        analogWrite(rightM1, 0);
        analogWrite(rightM0, right<<1);
    } else {
        analogWrite(rightM0, 0);
        analogWrite(rightM1, right<<1);
    }
    if (direction_left) {    // Move forward
        analogWrite(leftM1, 0);
        analogWrite(leftM0, left<<1);
    } else {
        analogWrite(leftM0, 0);
        analogWrite(leftM1, left<<1);
    }
}
