// Template script for the servo control. In this script  and servo handler is implemented
// Required hardware:
// - 1x Flight Recorder Board with ESP32
// - 1x Turnigy TGY IA6B Receiver programmed in PPL mode
// - 1x Tunigy TGY-i6S Controller programmed in PPL mode
// - 2x Servo motors
// - 1x RGB LED 
// Connections:
// - TGY channel PPM, pin 1 - FR Board, PPL2 GND
// - TGY channel PPM, pin 2 - FR Board, PPL2, D4
// - TGY channel PPM, pin 3 - FR Board, J11 5V (or connected to other 5V source)
// Required libraries:
// - FRLibBasics (download from https://github.com/josmeuleman/FRLibBasics, unzipped in ../Documents/Arduino/libraries/ )
// - FRLibIntegration (download from https://github.com/josmeuleman/FRLibIntegration, unzipped in ../Documents/Arduino/libraries/ )
// - ESP32Servo.h
//
// 2024-03-21, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

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
  myLed.SetColor(MAGENTA);
  Serial.begin(9600);  // Start the serial communciation
    
  myReceiver.Init();
  Serial.println(NUMBEROFSERVOS);

  for (int i=0; i<NUMBEROFSERVOS; i++) {
    float maxStep = MAXSERVOSPEEDDEGS[i]*LOOPTIMESERVOMS/1000.0;
    myServo[i].Init(PINSERVO[i], maxStep, SERVOSTARTPOS[i], SERVOENDPOS[i]);
  }

  myServoTimer.Start();
  Serial.println("End of Setup");
  myLed.SetColor(GREEN);  
}

//---------------------------------------------------------------------------------------------------------
// LOOP
// This block of code is looped infinitely
//---------------------------------------------------------------------------------------------------------
void loop() {
  //-------------------------------------------------------------------------------------------------------
  // Read the myReceiver and store the values
  //-------------------------------------------------------------------------------------------------------
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
  
  // Compare the state with the previous state:
  startLogger = (loggerSwitchState < loggerSwitchStatePrev);  //if new state is LOW (0) and old state is HIGH (1)
  stopLogger = (loggerSwitchState > loggerSwitchStatePrev);   //if new state is HIGH (1) and old state is LOW (1)
  loggerSwitchStatePrev = loggerSwitchState; //update the previous state for the next loop

  if  (startLogger) {
    // insert code for starting logging
    myLed.SetColor(BLUE);
    Serial.println("Start logging");
  }
  if (stopLogger) {
    // insert code for stopping logging
    myLed.SetColor(GREEN);
    Serial.println("Stop logging ");
  }

  // Read switch SWB (high/mid/low) for landing gear. 
  HandleLandingGearSwitch(); //Update of landingGearAction and landingGearHatchAction

  //-------------------------------------------------------------------------------------------------------
  // Handle the motors
  //-------------------------------------------------------------------------------------------------------
  UpdateServos();
  // // for debugging, print some values. This debugging slows down your program
  // for (int i = 0; i < NUMBEROFSERVOS; i++) {
  //   Serial.print(servoActualPos[i]);
  //   Serial.print("; ");
  // }
  // Serial.println();

  
  //-------------------------------------------------------------------------------------------------------
  // End of the loop
  //-------------------------------------------------------------------------------------------------------
  // Kill the time until
  if (myServoTimer.WaitUntilEnd()) {
    Serial.println("Overrun!");
  }
}

//---------------------------------------------------------------------------------------------------------
// FUNCTIONS
// Here the custom functions are defined
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
// Function that prints an error to the serial port and makes the RGBLED red
//---------------------------------------------------------------------------------------------------------
void Error(String errorMessage) {
  Serial.println(errorMessage);
  myLed.SetColor(RED);
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

//---------------------------------------------------------------------------------------------------------
// Function that controls the servos to their setpoints with limited speed
//---------------------------------------------------------------------------------------------------------
void UpdateServos(){
  for (int i = 0; i < NUMBEROFSERVOS; i++) {
    // Calculate the differrence between target and current position
    float posError = servoTargetPos[i] - servoActualPos[i];
    float maxStep = MAXSERVOSPEEDDEGS[i]*LOOPTIMESERVOMS/1000.0; //maximum step a servo can make in a loopcycle based on maximum speed
    if (posError > maxStep) {
      // if the difference is larger than the maximum step size, limit the step size
      posError = maxStep;
    }
    if (posError < -maxStep) {
      // same, but for negative values
      posError = -maxStep;
    }
    servoActualPos[i] = servoActualPos[i]+posError;

    // Write the rounded off setpoint to the servo motor
    //myServo[i].write(int(servoActualPos[i]));
  }
}