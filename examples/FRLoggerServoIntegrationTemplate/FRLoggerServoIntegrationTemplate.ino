// Template script integrating a servo loop and logger loop
// In this script the servo loop runs at a faster rate than the logger loop
// both run at the same time 
// Required hardware:
// - 1x ESP32
// Connections:

// Required libraries:
// - FRLibBasics (download from https://github.com/josmeuleman/FRLibBasics, unzipped in ../Documents/Arduino/libraries/ )
// - FRLibIntegration (download from https://github.com/josmeuleman/FRLibIntegration, unzipped in ../Documents/Arduino/libraries/ )
//
// 2024-03-21, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands
#include "init.h"

//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);  // Start the serial communciation

  myServoTimer.Start();
  myLoggerTimer.Start();
  Serial.println("End of Setup");
}

//---------------------------------------------------------------------------------------------------------
// LOOP
// This block of code is looped infinitely
//---------------------------------------------------------------------------------------------------------
void loop() {
  // Read the myReceiver and store the values
  
  // Process the controller channels
  
  // Handle the motors
  

  // Write to the log file if needed
  // Check if it is time to log the data
  if (myLoggerTimer.LoopTimePassed()) {
    // Write to the logger. This block of code will run only when the log time has passed
    Serial.print("Clock time: ");
    Serial.print(millis());
    Serial.print("; Number of times the Servoloop has run: ");
    Serial.println(nLoops);
    nLoops = 0;
  }

  // End of the loop
  nLoops++;
  if (myServoTimer.WaitUntilEnd()) {
    Serial.println("Overrun!");
  }
}
