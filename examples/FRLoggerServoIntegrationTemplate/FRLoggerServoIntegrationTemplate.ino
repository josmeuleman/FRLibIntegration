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

// Libraries from FRLibBasics
#include <FRTimer.h>

// Libraries from FRLibIntegration

// Other libraries

// Pins

// other constants
const int LOOPTIMESERVOMS = 10;     // Loop time for controlling servos
const int LOOPTIMELOGGERMS = 100;   // loop time for logging the sensors

// Create all objects
Timer myServoTimer(LOOPTIMESERVOMS);// Timer object for the clock
Timer myLoggerTimer(LOOPTIMELOGGERMS);  // Timer object for the clock

// define parameters
int nLoops = 0; // Just a counter for checking the loops

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
  //-------------------------------------------------------------------------------------------------------
  // Read the myReceiver and store the values
  //-------------------------------------------------------------------------------------------------------


  //-------------------------------------------------------------------------------------------------------
  // Process the controller channels
  //-------------------------------------------------------------------------------------------------------
  
  //-------------------------------------------------------------------------------------------------------
  // Handle the motors
  //-------------------------------------------------------------------------------------------------------


  //-------------------------------------------------------------------------------------------------------
  // Write to the log file if needed
  //-------------------------------------------------------------------------------------------------------
  // Check if it is time to log the data
  if (myLoggerTimer.LoopTimePassed()) {
    // Write to the logger. This block of code will run only when the log time has passed
    Serial.print("Clock time: ");
    Serial.print(millis());
    Serial.print("; Number of times the Servoloop has run: ");
    Serial.println(nLoops);
    nLoops = 0;

  }

  //-------------------------------------------------------------------------------------------------------
  // End of the loop
  //-------------------------------------------------------------------------------------------------------
  // Kill the time until
  nLoops++;
  if (myServoTimer.WaitUntilEnd()) {
    Serial.println("Overrun!");
  }
}

//---------------------------------------------------------------------------------------------------------
// Function that prints an error to the serial port and makes the RGBLED red
//---------------------------------------------------------------------------------------------------------
void Error(String errorMessage) {
  Serial.println(errorMessage);
}

