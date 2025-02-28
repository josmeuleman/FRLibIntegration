#ifndef INIT_H
#define INIT_H

//---------------------------------------------------------------------------------------------------------
// INIT
// Here libraries are loaded, and constants and variables are defined.
//---------------------------------------------------------------------------------------------------------

// Libraries from FRLibBasics
#include <FRTimer.h>

// other constants
const int LOOPTIMESERVOMS = 10;     // Loop time for controlling servos
const int LOOPTIMELOGGERMS = 100;   // loop time for logging the sensors

// Create all objects
Timer myServoTimer(LOOPTIMESERVOMS);// Timer object for the clock
Timer myLoggerTimer(LOOPTIMELOGGERMS);  // Timer object for the clock

// define parameters
int nLoops = 0; // Just a counter for checking the loops

//---------------------------------------------------------------------------------------------------------
// End of INIT
//---------------------------------------------------------------------------------------------------------
#endif //INIT_H
