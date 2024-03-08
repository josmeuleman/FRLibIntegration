// Testscript OverSampling & Timers on the Flight Recorder PCB
// Required hardware:
// - 1x potmeter
// - 6x breadboard cables female-male
// Connections:
// - Potmeter1, GND - Analog Input J6, GND
// - Potmeter1, Wiper - Analog Input J6, D35
// - Potmeter1, VCC - Analog Input J6, 3.3V
// Required libraries:
// - FRLib (download from https://github.com/josmeuleman/FRLib, unzipped in ../Documents/Arduino/libraries/ )
//
// 2023-03-14, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#include <FRTimer.h>

const int PINAD = 34;               // Analog input pin number
const int LOOPTIMEREADMS = 100;     // Loop time for reading the AD channel in milliseconds
const int LOOPTIMEPRINTMS = 1000;   // Loop time for printing average values to the Serial monitor

Timer myFastTimer(LOOPTIMEREADMS);
Timer mySlowTimer(LOOPTIMEPRINTMS);

int analogValue;
long sumAnalogValues = 0;
int counter = 0;
float averageValue = 0.0;
void setup() {
  Serial.begin(9600);     // Start the serial communciation
  pinMode(PINAD, INPUT);  // Assign the pin for input
  myFastTimer.Start();
  mySlowTimer.Start();
}

void loop() {
  // Loop through the two pins
  analogValue = analogRead(PINAD);  // Read the analog input
  sumAnalogValues += analogValue;   //add the new value to the total sum
  counter++;                        // increase the counter
  if (mySlowTimer.LoopTimePassed()) {
    averageValue = float(sumAnalogValues) / float(counter);
    counter = 0;          //Reset the counter
    sumAnalogValues = 0;  //Reset the sum value
  }
  Serial.print("CurrentValue:");
  Serial.print(analogValue);  // Print the current value
  Serial.print("; AverageValue:");
  Serial.println(averageValue); // Print the last average value
  if (myFastTimer.WaitUntilEnd()) {
    Serial.println("Overrun!");
  }
}