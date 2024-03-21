// Template script for recording  sensors to an SD card for Flight Recorder PCB v2
// This script logs a few sensors and can be extended with more
// Required hardware:
// - 1x Flight Recorder Board v2 with ESP32
// - 1x Angular Sensor (AS5600)
// - 1x IMU Sensor (MPU9250)
// - 1x GPS (GY-GPSV3-neo or Adafruit )
// - 1x SD Card reader
// Connections:
// - components mounted on board
// - SD card reader mounted on board
// Required libraries:
// - FRLibBasics (download from https://github.com/josmeuleman/FRLibBasics, unzipped in ../Documents/Arduino/libraries/ )
// - FRLibIntegration (download from https://github.com/josmeuleman/FRLibIntegration, unzipped in ../Documents/Arduino/libraries/ )
// - AS5600 by Rob Tillaart (tested on 0.6.0)
// - MPU9280 Bolder Flight Systems (tested on 5.6.0), dependencies:
//    - Eigen Bolder Flight Systems (tested on 3.0.2)
//    - Unit Conversions Bolder Flight Systems (tested on 5.0.0)
// - TinyGPSPlus.h (tested)
//
// 2024-03-21, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

// Libraries from FRLibBasics
#include <FRTimer.h>
#include <FRLogger.h>
#include <FRRGBLED.h>
#include <FRButton.h>

// Libraries from FRLibIntegration
#include <FRAS5600.h>   //library for logging AS5600 (angular sensor)
#include <FRMPU9250.h>  //library for logging MPU9250 (IMU)
#include <FRTinyGPS.h>  //library for logging TinyGPS

// Other libraries
#include <SSD1306Ascii.h>      //i2C OLED
#include <SSD1306AsciiWire.h>  //i2C OLED

// Pins
const byte I2C_SDA = 21;      // The data pin for I2C communication
const byte I2C_SCL = 22;      // The clock pin for I2C communcation
const byte PINSWITCH = 35;    // The pin number for he button to start and stop logging

// other constants
const int LOGGERLOOPTIMEMS = 100;   // Loop time for logging
const int OLED_ADDRESS = 0x3C;      // I2C address of OLED
// const float LAT0 = 52.31401298494572; //Latitude of null reference location
// const float LON0 = 4.620160388788039; //Longitude of null reference location
const float LAT0 = 51.998466760972; //Latitude of null reference location, in this case the API building
const float LON0 = 4.3696943967123; //Longitude of null reference location, in this case the API building

// Create all objects
Timer myLoggerTimer(LOGGERLOOPTIMEMS); // Timer object for the clock
Logger myLogger;                    // Logger object for logging sensors to the SD
Button myButton(PINSWITCH, true);   // Create a button object with the given pin. True for an inverted button, false for a normal button
RGBLED myLed;                       // Create a RGB led object. pinnummbers are defined in the library FRRGBLED.h.
FRAS5600 myAngleOfAttackSensor;
FRMPU9250 myIMUSensor;
FRTinyGPS myGPSSensor;
SSD1306AsciiWire myOLED;

//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  myLed.SetColor(MAGENTA);
  Serial.begin(9600);
  delay(1000);  // safe to wait a few milliseconds after serial.begin before writing to serial port
  Serial.println("Setup ");
  bool needOffsetCalculation = hasButtonBeenPressedDuringWait(4);
  if (needOffsetCalculation) {
    Serial.println("Offsets will be calculated during setup");
  } else {
    Serial.println("Default offset values will be used");
  }


  // Start the serial communication for all I2C sensors
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);

  // Initialize each sensor object. Give an error on failure
  if (!myAngleOfAttackSensor.Init()){
    Error("Angle of Attack Sensor (AS5600) not found!");
  }

  if (!myIMUSensor.Init(Wire)){
    Error("IMU (MPU9250) not found!");
  }

  if (!myGPSSensor.Init(LAT0, LON0)) {
    Error("GPS not found!");
  }

  if (!myLogger.CheckSD())
    Error("No SD card found!");
  
  myOLED.begin(&Adafruit128x32, OLED_ADDRESS);
  myOLED.setFont(Adafruit5x7);
  myOLED.clear();              //clear display

  // Wait a little to make sure all sensors are up and running before reading them out
  delay(100);

  
  // Add the sensors to the logger
  // The "&" sign means that the sensor gets the address of the sensor object (pointer)
  // The logger now can read updates from the sensor
  myLogger.AddSensor(&myAngleOfAttackSensor);
  myLogger.AddSensor(&myIMUSensor);
  myLogger.AddSensor(&myGPSSensor);
  
  // if the button was pressed at start, here the offset corrections can be made. In most cases it means: what you read now is zero
  // only for acceleration, be mindful that az = -9.81, assuming that your sensor is orientated as such
  if (needOffsetCalculation) {
    Serial.println("Sensor values before and after Calibration:");
    Serial.println(myAngleOfAttackSensor.HeaderString());
    Serial.println(myAngleOfAttackSensor.SensorString());
    myAngleOfAttackSensor.AutoOffset();
    //myAngleOfAttackSensor.SetOffsetAngle(90);
    Serial.println(myAngleOfAttackSensor.SensorString());
    
    Serial.println(myIMUSensor.HeaderString());
    Serial.println(myIMUSensor.SensorString());
    myIMUSensor.AutoOffsetGyro();
    myIMUSensor.SetOffsetAcc(0.0, 0.0, -9.81);
    Serial.println(myIMUSensor.SensorString());
   
  }

  
  
  // At the end of the setup, start the clock (for calculation loop durations)
  myLoggerTimer.Start();
  Serial.println("End of Setup");
  myLed.SetColor(GREEN);
}

//---------------------------------------------------------------------------------------------------------
// LOOP
// This block of code is looped infinitely
//---------------------------------------------------------------------------------------------------------
void loop() {

  //-------------------------------------------------------------------------------------------------------
  // Start or stop logger, depending on the button state
  //-------------------------------------------------------------------------------------------------------
  myButton.Update();              // Read the state of the button
  if (myButton.HasChangedUp()) {  //Check if the state has changed from low to high
    if (!myLogger.IsLogging()) {  // It wasn't logging yet, so start logging
      Serial.println("Start logging");
      if (!myLogger.StartLogger()) {
        Error("Something went wrong with the start of the log");
      } else { // the actual start of the logging
        myLed.SetColor(BLUE);
        Serial.print("File opened with the name: ");
        Serial.println(myLogger.GetLoggerFileName());
      }
    } else {  // Else we were logging, so now stop logging
      Serial.println("Stop logging");
      if (!myLogger.StopLogger()) {
        Error("Something went wrong with the stopping of the log");
      } else {
        myLed.SetColor(GREEN);
      }
    }
  }

  //-------------------------------------------------------------------------------------------------------
  // Write to the log file if needed
  //-------------------------------------------------------------------------------------------------------
  String myString = myLogger.UpdateSensors();  // Updates all connected sensors and generates a string of all sensor values;
  Serial.print(myString);  // Writing to the Serial Monitor will sometimes take more than 100 ms. That will give overruns So print to screen only when you have a slow update rate.
  myLogger.WriteLogger();  // This will only write to logger if myLogger.IsLogging is true;

  //-------------------------------------------------------------------------------------------------------
  // Update the OLED
  //-------------------------------------------------------------------------------------------------------
  myOLED.setRow(1);
  myOLED.print("# Sats found: "); 
  myOLED.print(myGPSSensor.GetSatellites());
  myOLED.println("   ");
  if (myGPSSensor.HasValidData()){
    myOLED.println("GPS fix    "); 
  }
  else {
    myOLED.println("No GPS fix"); 
  }
  //-------------------------------------------------------------------------------------------------------
  // End of the loop
  //-------------------------------------------------------------------------------------------------------
  // At the end of the loop, WaitUntilEnd runs until the time until looptime has passed
  if (myLoggerTimer.WaitUntilEnd()) {
    Serial.println("Overrun!"); // if there are delays in the loop, you will get overruns i.e. the loop took longer than the looptime
  }
  //
}

//---------------------------------------------------------------------------------------------------------
// FUNCTIONS
// Here the custom functions are defined
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
// Function that prints an error to the serial port and makes the RGBLED red
//---------------------------------------------------------------------------------------------------------
void Error(String errorMessage) {
  myLed.SetColor(RED);
  Serial.println(errorMessage);
}

//---------------------------------------------------------------------------------------------------------
// Function that waits maximum x seconds. it returns true if within x seconds the button has been pushed
//---------------------------------------------------------------------------------------------------------
bool hasButtonBeenPressedDuringWait(int waitTimeSec) {
  Serial.printf("press the button for offset calibration within %d seconds, ", waitTimeSec);
  Serial.println("else offset is not calibrated");
  // print a fancy bar to the screen. if the dots reach the end, the time is up
  byte lineLength = 80;
  for (byte i=0; i<=lineLength; i++){
    Serial.print("-");
  }
  Serial.println("|");
  
  long tStart = millis();
  bool isPressed = false;
  bool continueLoop = true;  // continue the loop until this is false
  while (continueLoop) {
    myButton.Update();
    
    if (myButton.HasChangedUp()) { // button has been pushed, return true and quit the while loop
      isPressed = true;
      continueLoop = false;
    }
    
    if ((millis() - tStart) > waitTimeSec * 1000) { // Time out, quite the while loop
      continueLoop = false;
    }
    delay(int(waitTimeSec*1000/lineLength));
    Serial.print('.');  // running bar
  }
  Serial.println();
  return isPressed; // returns true if the button was pressed during this process, else false
}
