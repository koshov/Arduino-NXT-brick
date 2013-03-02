// Program to count pulses per revolution in an automobile
// TODO: Includes a running average to insure proper RPM output
// TODO: Output a clean averaged 5v Sq wave of RPM
// NOTE: May need to go to (us) as opposed to (ms) for better resolution

int Cycle = 0;                  // set to 0 for PulseStartTime and set to 
                        //   1 for PulseEndTime
unsigned long PulseStartTime;   // Saves Start of pulse in ms
unsigned long PulseEndTime;     // Saves End of pulse in ms
unsigned long PulseTime;        // Stores dif between start and stop of pulse
unsigned long RPM = 0;          // RPM to ouptut (30*1000/PulseTime)

void setup()
{
 Serial.begin(9600);            // OPENS SERIAL PORT SETS DATA TO 9600 bps
 attachInterrupt(0, RPMPulse, RISING); // Attaches interrupt to Digi Pin 2
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

void loop()
{ 
 Serial.print("RPM = ");      // Output RPM for debug
 Serial.print(int(RPM));      // RPM debug output
 Serial.print(" ");
 delay(1000);                  // 1 sec delay for debug output
}

