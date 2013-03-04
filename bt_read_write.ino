#include <Wire.h>

int frontM0 = 11;    // Left motor black wire
int frontM1 = 10;    // Left motor white wire
int backM0 = 5;    // Right motor black wire
int backM1 = 6;    // Right motor white wire
int Cycle = 0;                  // set to 0 for PulseStartTime and set to 
unsigned long PulseStartTime;   // Saves Start of pulse in ms
unsigned long PulseEndTime;     // Saves End of pulse in ms
unsigned long PulseTime;        // Stores dif between start and stop of pulse
unsigned long RPM = 0;          // RPM to ouptut (30*1000/PulseTime)

void setup() {
    Wire.begin();
    Serial.begin(9600); 
    Serial.println("Hello!");

    pinMode(frontM0, OUTPUT);
    pinMode(frontM1, OUTPUT);
    pinMode(backM0, OUTPUT);
    pinMode(backM1, OUTPUT);
    attachInterrupt(0, RPMPulse, RISING);
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

void loop() {
    if (Serial.available()){
        byte ctrlVals[5];    // Array to store control values
        int i = 0;          // Counter for control package validation
        delay(5);           // Timeout for serial buffer consistency
        while (Serial.available() && i<5){
            ctrlVals[i] = Serial.read();
            i++;
        }
        if (i==5) {         // Control package is fine
            for (int i=0; i<5; i++){
                Serial.print(bitRead(ctrlVals[i], 7));
                Serial.print(" - ");
                // bitClear(ctrlVals[i], 7);
                Serial.print(ctrlVals[i]<<1);
                Serial.print("; ");
              }
            Serial.println();
            processVals(ctrlVals);
            Wire.beginTransmission(4);
            Wire.write(ctrlVals[0]);
            Wire.write(ctrlVals[1]);
            Wire.endTransmission();
            Serial.println("Tuk!");
            Serial.print("RPM= ");
            Serial.println(RPM);
        } else if (i>5){
            Serial.println("Control signal larger than 5 bytes");
        } else {
            Serial.println("Control signal less than 5 bytes");
        }
    }
}

void processVals(byte ctrlVals[5]){
    boolean direction_front = bitRead(ctrlVals[3], 7);
    bitClear(ctrlVals[3], 7);
    byte front = ctrlVals[3]<<1;

    boolean direction_back = bitRead(ctrlVals[2], 7);
    bitClear(ctrlVals[2], 7);
    byte back = ctrlVals[2]<<1;

    if (direction_front) {    // Move forward
        analogWrite(frontM1, 0);
        analogWrite(frontM0, front);
    } else {
        analogWrite(frontM0, 0);
        analogWrite(frontM1, front);
    }
    if (direction_back) {    // Move forward
        analogWrite(backM0, 0);
        analogWrite(backM1, back);
    } else {
        analogWrite(backM1, 0);
        analogWrite(backM0, back);
    }
}
