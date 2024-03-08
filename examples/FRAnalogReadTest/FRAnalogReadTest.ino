// Testscript for testing the analog input on the Flight Recorder PCB
// Required hardware:
// - 2x potmeter
// - 6x breadboard cables female-male
// Connections:
// - Potmeter1, GND - Analog Input J6, GND
// - Potmeter1, Wiper - Analog Input J6, D35
// - Potmeter2, VCC - Analog Input J6, 3.3V
// - Potmeter2, GND - Analog Input J7, GND
// - Potmeter2, Wiper - Analog Input J7, D34
// - Potmeter2, VCC - Analog Input J7, 3.3V
// Required libraries:
// - FRLib (download from https://github.com/josmeuleman/FRLib, unzipped in ../Documents/Arduino/libraries/ )
//
// 2023-03-04, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#include <FRGeneric.h>

const int PINAD[2] = {34, 35}; // Analog input pin numbers

void setup() {
  Serial.begin(9600); // Start the serial communciation
  // Assign the two pins for input
  for (int i=0; i<2; i++) {
    pinMode(PINAD[i], INPUT);
  }
}

void loop() {
  // Loop through the two pins
  for (int i=0; i<2; i++) {
    int val = analogRead(PINAD[i]); // Read the analog input

    // For the Serial plotter, it may be conventient if the variable has a name
    // This will give the name A1 and A2 to the signal
    Serial.print("A"); 
    Serial.print(i+1);
    Serial.print(":");
    Serial.print(createIntString(val));  // Print the value
    
  }
  Serial.println(); // New line
  delay(10);
}
