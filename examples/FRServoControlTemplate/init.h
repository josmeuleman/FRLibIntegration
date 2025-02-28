#ifndef INIT_H
#define INIT_H

//---------------------------------------------------------------------------------------------------------
// INIT
// Here libraries are loaded, and constants and variables are defined.
//---------------------------------------------------------------------------------------------------------

// Libraries from FRLibBasics
#include <FRTimer.h>
#include <FRButton.h>
#include <FRRGBLED.h>

// Libraries from FRLibIntegration
#include <FRPPMReceiver.h>

// Other libraries
#include <ESP32Servo.h>
#include "smoothservo.h"

const byte NUMBEROFSERVOS = 2;       // Number of servos 
const byte NUMBEROFCHANNELS = 8;     // Number of Channels of myReceiver

// Pins
const byte PINSERVO[NUMBEROFSERVOS] = {25, 26}; // Servo Channels
const byte PINPPM = 4;                // PM2 input pint
const byte PINSWITCH = 35;            // The pin number for he button to start and stop logging

// other constants
const int  LOOPTIMESERVOMS = 100;     // Loop time for controlling servos. Make it 10 ms for smoother control, but then remove the Serial.print statements to prevent overruns


// Create all objects
Timer myServoTimer(LOOPTIMESERVOMS);// Timer object for the clock
SmoothServo myServo[NUMBEROFSERVOS];      // create a servo object
RGBLED myLed;                       // Create a RGB led object. pinnummbers are defined in the library FRRGBLED.h.
FRPPMReceiver myReceiver(PINPPM, NUMBEROFCHANNELS);  // Create a PPM receiver object with given pin and number of channels
Button myButton(PINSWITCH, true);       // Create a button object with the given pin. True for an inverted button, false for a normal button

int channelValues[NUMBEROFCHANNELS];
const byte LOGGERSWITCHCHANNEL = 4;
const byte LANDINGGEARCHANNEL = 5;

// States for starting and stopping the logger
bool loggerSwitchState = HIGH;
bool loggerSwitchStatePrev = HIGH;
bool startLogger = false;
bool stopLogger = false;
bool servoTargetUp = true;

// States for landing gear
triStateSwitch landingGearSwitchState;
triStateSwitch landingGearSwitchStatePrev;

// Servo parameters: number, max speed, max values;
const byte SERVOLANDINGGEAR = 0;        // the servo number of the landing gear
const byte SERVOLANDINGHATCH = 1;   // the servo number of the landing gear hatch
const int MAXSERVOSPEEDDEGS[NUMBEROFSERVOS] = {30, 30}; // Maximum speed of the servos in degrees per sec
const byte SERVOSTARTPOS[NUMBEROFSERVOS] = {130, 27}; //The starting position of the servos, type in your values here
const byte SERVOENDPOS[NUMBEROFSERVOS] = {13, 176}; //The end position of the servos, type in your values here

//---------------------------------------------------------------------------------------------------------
// End of INIT
//---------------------------------------------------------------------------------------------------------
#endif //INIT_H