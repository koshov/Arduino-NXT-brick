#include <Wire.h>

int Cycle=0;
unsigned long PulseStartTime;   // Saves Start of pulse in ms
unsigned long PulseEndTime;     // Saves End of pulse in ms
unsigned long PulseTime;        // Stores dif between start and stop of pulse
unsigned long RPM = 0;          // RPM to ouptut (30*1000/PulseTime)
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
    attachInterrupt(0, RPMPulse, RISING);
}

void loop()
{
   Serial.print("RPM = ");      // Output RPM for debug
   Serial.print(int(RPM));      // RPM debug output
   Serial.print(" ");
   delay(1000);   
}


void RPMPulse()
{
  if (Cycle == 0)                // Check to see if start pulse
  {
    PulseStartTime = millis();  // stores start time
    Cycle = 1;           // sets counter for start of pulse
    return;                     // a return so it doesnt run the next if
  }
  if (Cycle == 1)             // Check to see if end pulse
  {
    detachInterrupt(0);         // Turns off inturrupt for calculations
    PulseEndTime = millis();    // stores end time
    Cycle = 0;                  // resets counter for pulse cycle
    calcRPM();                  // call to calculate pulse time
  }
}

void calcRPM()
{
  PulseTime = PulseEndTime - PulseStartTime; // Gets pulse duration
  Serial.print("PulseTime =");               // Output pulse time for debug
  Serial.print(PulseTime);                   // Pulse debug output
  Serial.print(" ");                         
  RPM = 30*1000/PulseTime*2;                 // Calculates RPM
  attachInterrupt(0, RPMPulse, RISING);      // re-attaches interrupt to Digi Pin 2
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
