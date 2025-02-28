#ifndef INIT_H
#define INIT_H

// Libraries from FRLibBasics
#include <FRTimer.h>
#include <FRButton.h>
#include <FRRGBLED.h>

// Libraries from FRLibIntegration
#include <FRPPMReceiver.h>

// Other libraries
#include <ESP32Servo.h>

const byte NUMBEROFSERVOS = 2;       // Number of servos 
const byte NUMBEROFCHANNELS = 8;     // Number of Channels of myReceiver

// Pins
const byte PINSERVO[NUMBEROFSERVOS] = {25, 26}; // Servo Channels
const byte PINPPM = 4;               // PM2 input pint

// other constants
const int  LOOPTIMESERVOMS = 10;     // Loop time for controlling servos


// Create all objects
Timer myServoTimer(LOOPTIMESERVOMS);// Timer object for the clock
Servo myServo[NUMBEROFSERVOS];      // create a servo object
RGBLED myLed;                       // Create a RGB led object. pinnummbers are defined in the library FRRGBLED.h.
FRPPMReceiver myReceiver(PINPPM, NUMBEROFCHANNELS);  // Create a PPM receiver object with given pin and number of channels

int channelValues[NUMBEROFCHANNELS];
const byte LOGGERSWITCHCHANNEL = 4;
const byte LANDINGGEARCHANNEL = 5;

// States for starting and stopping the logger
bool loggerSwitchState = HIGH;
bool loggerSwitchStatePrev = HIGH;
bool startLogger = false;
bool stopLogger = false;

// States for landing gear
triStateSwitch landingGearSwitchState;
triStateSwitch landingGearSwitchStatePrev;

// Servo parameters: number, max speed, max values;
const byte SERVOLANDINGGEAR = 0;        // the servo number of the landing gear
const byte SERVOLANDINGHATCH = 1;   // the servo number of the landing gear hatch
const int MAXSERVOSPEEDDEGS[NUMBEROFSERVOS] = {30, 30}; // Maximum speed of the servos in degrees per sec
int servoTargetPos[NUMBEROFSERVOS]; 
float servoActualPos[NUMBEROFSERVOS];
const int SERVOLANDINGGEARPOSEXTENDED = 130;
const int SERVOLANDINGGEARPOSRETRACTED = 13;
const int SERVOLANDINGHATCHPOSOPEN = 27;
const int SERVOLANDINGHATCHPOSCLOSED = 176;

#endif //INIT_H