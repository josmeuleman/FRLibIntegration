#ifndef INIT_H
#define INIT_H

// Libraries from FRLibBasics
#include <FRTimer.h>
#include <FRLogger.h>
#include <FRRGBLED.h>
#include <FRButton.h>

// Libraries from FRLibIntegration
#include <FRMPU9250.h>  //wrapper library for logging MPU9250 (IMU)
#include <FRBMP280.h>   //wrapper library for logging BMP280
#include <FRTinyGPS.h>  //wrapper library for logging TinyGPS

// Other libraries
#include <SSD1306Ascii.h>      //i2C OLED
#include <SSD1306AsciiWire.h>  //i2C OLED


// Pins
const byte I2C_SDA = 21;    // The data pin for I2C communication
const byte I2C_SCL = 22;    // The clock pin for I2C communcation
const byte PINSWITCH = 35;  // The pin number for he button to start and stop logging

// other constants
const int LOGGERLOOPTIMEMS = 100;  // Loop time for logging
const int OLED_ADDRESS = 0x3C;     // I2C address of OLED

const float LAT0 = 51.99751239776191;  //Latitude of null reference location, in this case the Terminal
const float LON0 = 4.369074612612849;  //Longitude of null reference location, in this case the Terminal

// Create all objects
Timer myLoggerTimer(LOGGERLOOPTIMEMS);  // Timer object for the clock
Logger myLogger;                        // Logger object for logging sensors to the SD
Button myButton(PINSWITCH, true);       // Create a button object with the given pin. True for an inverted button, false for a normal button
RGBLED myLED;                           // Create a RGB led object. pinnummbers are defined in the library FRRGBLED.h.
FRMPU9250 myIMUSensor;
FRBMP280 myAltitudeSensor;
FRTinyGPS myGPSSensor;
SSD1306AsciiWire myOLED;

#endif //INIT_H