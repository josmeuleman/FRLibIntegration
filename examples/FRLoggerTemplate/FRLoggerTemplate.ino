// Template script for recording  sensors to an SD card for Flight Recorder PCB v2
// This script logs a few sensors and can be extended with more
// Required hardware:
// - 1x Flight Recorder Board v2 with ESP32
// - 1x Pressure Sensor (BMP280)
// - 1x IMU Sensor (MPU9250)
// - 1x GPS (GY-GPSV3-neo or Adafruit )
// - 1x External board with SD, OLED and Button
// Connections:
// - components mounted on board
// Required libraries:
// - FRLibBasics (download from https://github.com/josmeuleman/FRLibBasics)
// - FRLibIntegration (download from https://github.com/josmeuleman/FRLibIntegration)
//    - all FRLibIntegration dependencies
//
// 2025-02-28, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

// these are local files that contain big chunks of code to make the main document more readable. 
// Check the corresponding files in the tab
#include "init.h"
#include "customfunctions.h"

//---------------------------------------------------------------------------------------------------------
// SETUP
//---------------------------------------------------------------------------------------------------------
void setup() {
  // Open the serial port
  Serial.begin(9600);

  // Start the serial communication for all I2C sensors and display
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);

  // init the OLED
  myOLED.begin(&Adafruit128x32, OLED_ADDRESS);
  myOLED.setFont(Adafruit5x7);  // use System5x7 for small letters
  myOLED.clear();               //clear display
  
  // Write messages to Serial and OLED, and control the RGB LED)
  Message("Setup", myLED, MAGENTA, Serial, myOLED);
  
  // Check if the button has been pressed in the following X seconds
  bool needOffsetCalculation = hasButtonBeenPressedDuringWait(4, myButton, Serial, myOLED);

  // Initialize all sensors
  if (!myAltitudeSensor.Init(Wire)){
    Message("Altitude Sensor (BMP280) not found!", myLED, RED, Serial, myOLED);
  }
  if (!myIMUSensor.Init(Wire)) {
    Message("IMU (MPU9250) not found!", myLED, RED, Serial, myOLED);
  }
  if (!myGPSSensor.Init(LAT0, LON0)) {
    Message("GPS not found!", myLED, RED, Serial, myOLED);
  }
  if (!myLogger.CheckSD()) {
    Message("No SD card found!", myLED, RED, Serial, myOLED);
  }

  // Wait a little to make sure all sensors are up and running before reading them out
  delay(100);


  // Add the sensors to the logger
  // The "&" sign means that the sensor gets the address of the sensor object (pointer)
  // The logger now can read updates from the sensor
  myLogger.AddSensor(&myAltitudeSensor);
  myLogger.AddSensor(&myIMUSensor);
  myLogger.AddSensor(&myGPSSensor);

  // if the button was pressed at start, here the offset corrections can be made. In most cases it means: what you read now is zero
  // only for acceleration, be mindful that az = -9.81, assuming that your sensor is orientated as such
  if (needOffsetCalculation) {
    CalibrateSensors(myAltitudeSensor, myIMUSensor);
  }

  // At the end of the setup, start the clock (for calculation loop durations)
  myLoggerTimer.Start();
  Message("End of Setup.", myLED, GREEN, Serial, myOLED);
  myOLED.clear();
}

//---------------------------------------------------------------------------------------------------------
// LOOP
//---------------------------------------------------------------------------------------------------------
void loop() {
  // Start or stop logger, depending on the button state
  StartStopLogger(myButton, myLED, myLogger, Serial, myOLED);

  // Write to the log file if needed
  String myString = myLogger.UpdateSensors();  // Updates all connected sensors and generates a string of all sensor values;
  //Serial.print(myString);                      // Writing to the Serial Monitor will sometimes take more than 100 ms. That will give overruns So print to screen only when you have a slow update rate.
  myLogger.WriteLogger();                      // This will only write to logger if myLogger.IsLogging is true;

  // Update the OLED with a custom function 
  PrintGPSStatusToOLED(myGPSSensor, myOLED);
  
  // At the end of the loop, WaitUntilEnd runs until the time until looptime has passed
  if (myLoggerTimer.WaitUntilEnd()) {
    Serial.println("Overrun!");  // if there are delays in the loop, you will get overruns i.e. the loop took longer than the looptime
  }
}
