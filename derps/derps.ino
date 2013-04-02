#define legoSensorPin 12 // digital pin D14 is analog pin A0
#define legoSensor A1 // this time we define the analog channel

int readings = 0;
int counter = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("Start");
}

void loop(){
  int raw=ReadLegoSensor(); // we read the raw value of the sensor
  
  readings += raw;
  counter += 1;
  if (counter == 100){
    Serial.print("Raw value: ");
    Serial.println(raw);
      // and print it to the monitor
    counter = 0;
    readings = 0;
  }
}

int ReadLegoSensor(){
  pinMode(legoSensorPin, OUTPUT); //set pin as output
  digitalWrite(legoSensorPin, HIGH); //set output high
  delay(10); // wait a bit
  pinMode(legoSensorPin, INPUT); //set pin as input
  int value=analogRead(legoSensor); //read the input
  return value; //return the raw value
}
