// Template script for the servo control. In this script  and servo handler is implemented
// Required hardware:
// - 1x Flight Recorder Board with ESP32
// - 1x Turnigy TGY IA6B Receiver programmed in PPL mode
// - 1x Tunigy TGY-i6S Controller programmed in PPL mode
// - 2x Servo motors
// - 1x RGB LED 
// Connections:
// - TGY channel PPM, pin 1 - FR Board, PPL2 GND
// - TGY channel PPM, pin 2 - FR Board, J11 5V (or connected to other 5V source)
// - TGY channel PPM, pin 3 - FR Board, PPL2, D4
// Required libraries:
// - FRLibBasics (download from https://github.com/josmeuleman/FRLibBasics, unzipped in ../Documents/Arduino/libraries/ )
// - FRLibIntegration (download from https://github.com/josmeuleman/FRLibIntegration, unzipped in ../Documents/Arduino/libraries/ )
// - ESP32Servo.h
//
// 2025-02-28, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

// These are local files that contain big chunks of code to make the main document more readable. 
// Check the corresponding files in the tab
#include "init.h"
#include "customfunctions.h"
#include "smoothservo.h"


//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);  // Start the serial communciation
    
  myReceiver.Init();
  Serial.println(NUMBEROFSERVOS);

  for (int i=0; i<NUMBEROFSERVOS; i++) {
    float maxStep = MAXSERVOSPEEDDEGS[i]*LOOPTIMESERVOMS/1000.0;
    myServo[i].Init(PINSERVO[i], maxStep, SERVOSTARTPOS[i], SERVOENDPOS[i]);
  }

  myServoTimer.Start();
  Serial.println("End of Setup");
}

//---------------------------------------------------------------------------------------------------------
// LOOP
// This block of code is looped infinitely
//---------------------------------------------------------------------------------------------------------
void loop() {
  // Read the myReceiver and store the values
  for (byte i = 0; i < NUMBEROFCHANNELS; i++) {
    channelValues[i] = myReceiver.ReadChannel(i);
  }
  // for debugging, print some values. This debugging slows down your program
  // for (byte i = 0; i < NUMBEROFCHANNELS; i++) {
  //   Serial.print(channelValues[i]);
  //   Serial.print("; ");
  // }
  // Serial.println();

  //-------------------------------------------------------------------------------------------------------
  // Process the controller channels
  //-------------------------------------------------------------------------------------------------------
  // Read switch SWA (high/low) for logging
  loggerSwitchState = myReceiver.IsChannelHigh(LOGGERSWITCHCHANNEL);
  HandleLoggerSwitch(loggerSwitchState, loggerSwitchStatePrev);
  loggerSwitchStatePrev = loggerSwitchState; //update the previous state for the next loop

  // Read switch SWC (high/mid/low) for landing gear.
  landingGearSwitchState = myReceiver.GetChannelTriState(LANDINGGEARCHANNEL);
  // Check if landing gear and hatch need to move
  HandleMainLandingGearSwitch(landingGearSwitchState, landingGearSwitchStatePrev, myServo[SERVOLANDINGGEAR], myServo[SERVOLANDINGHATCH]);
  landingGearSwitchStatePrev = landingGearSwitchState; //update the previous state for the next loop

  // Update the smooth servos. There are some debugging print statements in the Update function you want to remove to speed up the code. 
  for (int i = 0; i < NUMBEROFSERVOS; i++) {
    myServo[i].Update();
  }
  Serial.println(); // Remove this print statement is you remove the prints in the Servo update 
  
  // At the end of the loop, WaitUntilEnd runs until the time until looptime has passed
  if (myServoTimer.WaitUntilEnd()) {
    Serial.println("Overrun!");
  }
}

//---------------------------------------------------------------------------------------------------------
// Function that controls the leading gear and hatch, based on the switch state
//---------------------------------------------------------------------------------------------------------

void HandleLandingGearSwitch(){
  // landingGearSwitchState can have the state LOSTATE (-1), MIDSTATE (0), or HISTATE (1)
  // the function GetChannelTriState returns one of these three states depending on the value of the channel
  landingGearSwitchState = myReceiver.GetChannelTriState(LANDINGGEARCHANNEL);

  if (landingGearSwitchState < landingGearSwitchStatePrev) {
    // towards rectracted state
    if (landingGearSwitchState == MIDSTATE) {
      // So it was HISTATE, now pull in the gear
      //servoTargetPos[SERVOLANDINGGEAR] = SERVOLANDINGGEARPOSRETRACTED;
      Serial.println("Retract gear");
    } else {
      // So it was in MIDSTATE, now close the hatch
      //servoTargetPos[SERVOLANDINGHATCH] = SERVOLANDINGHATCHPOSCLOSED;
      Serial.println("Close hatch");
    }
  }
  if (landingGearSwitchState > landingGearSwitchStatePrev) {
    // towards extended state
    if (landingGearSwitchState == MIDSTATE) {
      // So it was LOSTATE, now open the hatch
      //servoTargetPos[SERVOLANDINGHATCH] = SERVOLANDINGHATCHPOSOPEN;
      Serial.println("Open hatch");
    } else {
      // So it was in MIDSTATE, now extend the gear
      //servoTargetPos[SERVOLANDINGGEAR] = SERVOLANDINGGEARPOSEXTENDED;
      Serial.println("Extend gear");
    }
  }
  landingGearSwitchStatePrev = landingGearSwitchState; //update the previous state for the next loop

}
