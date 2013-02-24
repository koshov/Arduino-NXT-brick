char INBYTE;
int ctrlVals[5];

void setup() {
    Serial.begin(9600); 
}

void loop() {
    if (Serial.available()){
        delay(5);
        int i = 0;
        while (Serial.available() && i<5){
            ctrlVals[i] = Serial.read();
            i++;
        }
        if (i==5) {
            for (int i=0; i<5; i++){
                Serial.print(ctrlVals[i]);
                Serial.print('\t');
            }
            Serial.println();
        } else if (i>5){
            Serial.println("Control signal larger than 5 bytes");
        } else {
            Serial.println("Control signal less than 5 bytes");
        }
    }
   
    
}
