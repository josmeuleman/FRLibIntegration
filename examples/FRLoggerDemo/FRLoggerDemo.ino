// Testscript for Recording MPU and analog data to an SD card for  Flight Recorder PCB
// Required hardware:
// - 1x Flight Recorder Board with ESP32
// - 1x MPU6050
// - 1x SD Card reader
// - 1x Potmeter
// Connections:
// - Potmeter1, GND - Analog Input J6, GND
// - Potmeter1, Wiper - Analog Input J6, D35
// - Potmeter1, VCC - Analog Input J6, 3.3V
// - GPS mounted on board
// - SD card reader mounted on board
// Required libraries:
// - FRLib (download from https://github.com/josmeuleman/FRLib, unzipped in ../Documents/Arduino/libraries/ )
// - Adafruit_MPU6050.h
//
// 2023-04-05, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands


#include <FRTimer.h>
#include <FRLogger.h>
#include <FRLED.h>
#include <FRButton.h>
#include <FRMPU6050Manager.h> //Use a custom library that internally initializes and logs the MPU6050 so you don't need #include <Adafruit_MPU6050.h>  
#include <FRAnalogInputManager.h>


const int I2C_SDA = 33;                 // The data pin for I2C communication
const int I2C_SCL = 32;                 // The clock pin for I2C communcation
const int PINSWITCH = 22;               // The pin number for he button to start and stop logging
const int PINLED = 21;                  // The pin number for the LED
const int PINAD = 34;                   // Analog input pin number
const int LOOPTIMEMS = 100;             // Loop time for reading the AD channel in milliseconds

// Create all objects
Timer myTimer(LOOPTIMEMS);              // Timer object for the clock
Logger myLogger;                        // Logger object for logging sensors to the SD
Button myButton(PINSWITCH, true);       // Create a button object with the given pin. True for an inverted button, false for a normal button
LED myLed(PINLED);                      // Create a led object with the given pin.
MPU6050Manager myMPU;                   // Make an object for the sensor manager for the MPU6050 (accelerometer and gyro)
AnalogInputManager myAnalog1(PINAD, "AlphaVane[deg]");            


//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);     // Start the serial communciation
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);
  
  // The Wire connection must be made before initializing the MPU
  if (!myMPU.Init(Wire, MPU6050_RANGE_4_G, MPU6050_RANGE_500_DEG)) {
    Error("MPU not found!");
  }

  if (!myLogger.CheckSD()) {
    Error("No SD card found!");
  }
  
  myAnalog1.SetOutputRange(-135.0, 135.0);
  
  myLogger.AddSensor(&myAnalog1); // Add the analog sensor to the logger. the "&" means that the logger watches the address of the analog sensor 
  myLogger.AddSensor(&myMPU);

  myTimer.Start();
  
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
        myLed.SetOn();
        Serial.print("File opened with the name: ");
        Serial.println(myLogger.GetLoggerFileName());
      }
    } else {// Stop logging
      Serial.println("Stop logging");
      if (!myLogger.StopLogger()) {
        Error("Something went wrong with the stopping of the log");
      }
      else {
        myLed.SetOff();
      }
    }
  }

  String myString = myLogger.UpdateSensors(); // Updates all connected sensors and generates a string of all sensor values;
  //Serial.print(myString); // Writing to the Serial Monitor will sometimes take more than 100 ms. So print to screen only when you have a slow update rate. 
  myLogger.WriteLogger(); // Only writes to logger if myLogger. IsLogging is true;

  if (myTimer.WaitUntilEnd()) {
    Serial.println("Overrun!");
  }
}

//---------------------------------------------------------------------------------------------------------
// FUNCTIONS
// Here the custom functions are defined
//---------------------------------------------------------------------------------------------------------
void Error(String errorMessage){
  Serial.println(errorMessage);
  myLed.SetBlink(100);
}