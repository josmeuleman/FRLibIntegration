// Testscript for recording sensors to an SD card for Flight Recorder PCB v2
// This script logs a few sensors and can be extended with more
// Required hardware:
// - 1x Flight Recorder Board v2 with ESP32 
// - 1x Angular Sensor (AS5600)
// - 1x Altitude Sensoor (BMP280)
// - 1x SD Card reader
// Connections:
// - components mounted on board 
// - SD card reader mounted on board
// Required libraries:
// - FRLibBasics (download from https://github.com/josmeuleman/FRLibBasics, unzipped in ../Documents/Arduino/libraries/ )
// - FRLibIntegration (download from https://github.com/josmeuleman/FRLibIntegration, unzipped in ../Documents/Arduino/libraries/ )
// - Adafruit_MPU6050.h
//
// 2023-04-05, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands


// Libraries from FRLibBasics
#include <FRTimer.h>
#include <FRLogger.h>
#include <FRRGBLED.h>
#include <FRButton.h>
// Libraries from FRLibIntegration
#include <FRAS5600.h> //special library logging for AS5600
#include <FRBMP280.h> //special library logging for BMP280


const byte I2C_SDA = 21;                // The data pin for I2C communication
const byte I2C_SCL = 22;                // The clock pin for I2C communcation
const int PINSWITCH = 35;               // The pin number for he button to start and stop logging
const int LOOPTIMEMS = 100;             // Loop time for reading the AD channel in milliseconds

// Create all objects
Timer myTimer(LOOPTIMEMS);              // Timer object for the clock
Logger myLogger;                        // Logger object for logging sensors to the SD
Button myButton(PINSWITCH, true);       // Create a button object with the given pin. True for an inverted button, false for a normal button
RGBLED myLed;                           // Create a RGB led object. pinnummbers are defined in the library FRRGBLED.h.
FRBMP280 myAltitudeSensor;
FRAS5600 myAngleOfAttackSensor;    


//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  myLed.SetColor(MAGENTA);
  Serial.begin(9600);
  Serial.println("Setup ");
  
  // Start the serial communciation for all I2C sensors
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);
  
  // Initialize each sensor object. Give an error on failure
  if (!myAltitudeSensor.Init(Wire)) {
    Error("Altitude sensor (BMP280) not found!");
  }
  if (!myAngleOfAttackSensor.Init()) {
    Error("Angle of Attack Sensor (AS5600) not found!");
  }
  if (!myLogger.CheckSD()) {
    Error("No SD card found!");
  }
    
  // Add the sensors to the logger
  // The "&" sign means that the sensor gets the address of the sensor object (pointer)
  // The logger now can read updates from the sensor
  myLogger.AddSensor(&myAltitudeSensor);
  myLogger.AddSensor(&myAngleOfAttackSensor);


  myTimer.Start();
  Serial.println("End of Setup");
  myLed.SetColor(GREEN);
}

//---------------------------------------------------------------------------------------------------------
// LOOP
// This block of code is looped infinitely
//---------------------------------------------------------------------------------------------------------
void loop() {
  myButton.Update();              // Read the state of the button

  if (myButton.HasChangedUp()) {  //Check if the state has changed from low to high
    if (!myLogger.IsLogging()) { // Start logging
      Serial.println("Start logging");
      if (!myLogger.StartLogger()) {
        Error("Something went wrong with the start of the log");
      }
      else {
        myLed.SetColor(BLUE);
        Serial.print("File opened with the name: ");
        Serial.println(myLogger.GetLoggerFileName());
      }
    } else {// Stop logging
      Serial.println("Stop logging");
      if (!myLogger.StopLogger()) {
        Error("Something went wrong with the stopping of the log");
      }
      else {
        myLed.SetColor(GREEN);
      }
    }
  }

  String myString = myLogger.UpdateSensors(); // Updates all connected sensors and generates a string of all sensor values;
  Serial.print(myString); // Writing to the Serial Monitor will sometimes take more than 100 ms. So print to screen only when you have a slow update rate. 
  myLogger.WriteLogger(); // Only writes to logger if myLogger.IsLogging is true;

  if (myTimer.WaitUntilEnd()) {
    Serial.println("Overrun!");
  }
}

//---------------------------------------------------------------------------------------------------------
// FUNCTIONS
// Here the custom functions are defined
//---------------------------------------------------------------------------------------------------------
void Error(String errorMessage){
  myLed.SetColor(RED);
  Serial.println(errorMessage);
}



