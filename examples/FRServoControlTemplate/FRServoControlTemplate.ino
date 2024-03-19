// Template script for the servo control. In this script  and servo handler is implemented
// Required hardware:
// - 1x Flight Recorder Board with ESP32
// - 1x Turnigy TGY IA6B Receiver programmed in PPL mode
// - 1x Tunigy TGY-i6S Controller programmed in PPL mode
// - 2x Servo motors 
// Connections:
// - TGY channel PPM, pin 1 - FR Board, PPL2 GND
// - TGY channel PPM, pin 2 - FR Board, PPL2, D4
// - TGY channel PPM, pin 3 - FR Board, J11 5V (or connected to other 5V source)
// Required libraries:
// - FRLibBasics (download from https://github.com/josmeuleman/FRLibBasics, unzipped in ../Documents/Arduino/libraries/ )
// - FRLibIntegration (download from https://github.com/josmeuleman/FRLibIntegration, unzipped in ../Documents/Arduino/libraries/ )
// - ESP32Servo.h
//
// 2023-04-20, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#include <ESP32Servo.h>
#include <FRTimer.h>
#include <FRLogger.h>
#include <FRLED.h>
#include <FRButton.h>
#include <FRPPMReceiver.h>

// Some switches have three states. We make constants defining LOSTATE (-1), MIDSTATE (0) and HISTATE(1)
// typedef enum triStateSwitch {
//   LOSTATE = -1,
//   MIDSTATE,
//   HISTATE
// };

const byte NUMBEROFSERVOS = 4;       // Number of servos 
const byte NUMBEROFCHANNELS = 8;     // Number of Channels of myReceiver

const byte PINSWITCH = 35;           // The pin number for he button to start and stop logging
const byte PINLED = 12;              // The pin number for the LED
const byte PINSERVO[NUMBEROFSERVOS] = {25, 26, 27, 14}; // Servo Channels
const byte PINPPM = 4;               // PM2 input pint

const int  LOOPTIMELOGGERMS = 100;   // Loop time for reading the AD channel in milliseconds
const int  LOOPTIMESERVOMS = 100;     // Loop time for reading the AD channel in milliseconds


// Create all objects
Timer loggerTimer(LOOPTIMELOGGERMS);  // Timer object for the clock
Timer servoTimer(LOOPTIMESERVOMS);    // Timer object for the clock

Logger myLogger;                    // Logger object for logging sensors to the SD
Button myButton(PINSWITCH, true);   // Create a button object with the given pin. True for an inverted button, false for a normal button
LED myLed(PINLED);                  // Create a led object with the given pin.
Servo myServo[NUMBEROFSERVOS];      // create a servo object

FRPPMReceiver myReceiver(PINPPM, NUMBEROFCHANNELS);  // Create a PPM receiver object with given pin and number of channels
int channelValues[NUMBEROFCHANNELS];
// Suggestion for signal mapping
// channel0 - tick up --> handle cargo drop servo
// channel1 - NA
// channel2 - NA
// channel3 - NA
// channel4 - SWA UP/DOWN -> stop/start logging
// channel5 - SWB UP/MID/DOWN -> gears down & hatch open / gears up & hatch open / gears up & hatch closed
// channel6 - NA
// channel7 - SWD UP/DOWN -> cargo bay closed/open
const byte LOGGERSWITCHCHANNEL = 4;
const byte LANDINGGEARCHANNEL = 5;

// // Threshold for discrete PPM signals
// const int PPMTHRESHLOW = 850;
// const int PPMTHRESHMID = 1100;
// const int PPMTHRESHHIGH = 1350;

// States for starting and stopping the logger
bool loggerSwitchState = HIGH;
bool loggerSwitchStatePrev = HIGH;
bool startLogger = false;
bool stopLogger = false;

// States for landing gear
triStateSwitch landingGearSwitchState;
triStateSwitch landingGearSwitchStatePrev;


const byte SERVOLANDINGGEAR = 0;        // the servo number of the landing gear
const byte SERVOLANDINGHATCH = 1;   // the servo number of the landing gear hatch
const int MAXSERVOSPEEDDEGS[NUMBEROFSERVOS] = {30, 30, 100, 100}; // Maximum speed of the servos in degrees per sec
int servoTargetPos[NUMBEROFSERVOS] = {0, 0, 0, 0}; //The initial state of the servos
float servoActualPos[NUMBEROFSERVOS];
const int SERVOLANDINGGEARPOSEXTENDED = 180;
const int SERVOLANDINGGEARPOSRETRACTED = 0;
const int SERVOLANDINGHATCHPOSOPEN = 180;
const int SERVOLANDINGHATCHPOSCLOSED = 0;



//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  myLed.SetOff();
  Serial.begin(9600);  // Start the serial communciation
    
  myReceiver.Init();

  loggerTimer.Start();
  servoTimer.Start();

  for (int i=0; i<NUMBEROFSERVOS; i++) {
    myServo[i].attach(PINSERVO[i]);
  }

  // Starting pos for servos
  servoTargetPos[SERVOLANDINGGEAR] = SERVOLANDINGGEARPOSEXTENDED;
  servoTargetPos[SERVOLANDINGHATCH] = SERVOLANDINGHATCHPOSOPEN;
  for (int i = 0; i < NUMBEROFSERVOS; i++) {
    servoActualPos[i] = servoTargetPos[i];
  }
  

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
  if (servoTimer.WaitUntilEnd()) {
    Serial.println("Overrun!");
  }
}

//---------------------------------------------------------------------------------------------------------
// FUNCTIONS
// Here the custom functions are defined
//---------------------------------------------------------------------------------------------------------
void Error(String errorMessage) {
  Serial.println(errorMessage);
  myLed.SetBlink(100);
}


void HandleLandingGearSwitch(){
  // landingGearSwitchState can have the state LOSTATE (-1), MIDSTATE (0), or HISTATE (1)
  // the functin GetChannelTriState returns one of these three states depending on the value of the channel
  landingGearSwitchState = myReceiver.GetChannelTriState(LANDINGGEARCHANNEL);

  if (landingGearSwitchState < landingGearSwitchStatePrev) {
    // towards rectracted state
    if (landingGearSwitchState == MIDSTATE) {
      // So it was HISTATE, now pull in the gear
      servoTargetPos[SERVOLANDINGGEAR] = SERVOLANDINGGEARPOSRETRACTED;
      Serial.println("Retract gear");
    } else {
      // So it was in MIDSTATE, now close the hatch
      servoTargetPos[SERVOLANDINGHATCH] = SERVOLANDINGHATCHPOSCLOSED;
      Serial.println("Close hatch");
    }
  }
  if (landingGearSwitchState > landingGearSwitchStatePrev) {
    // towards extended state
    if (landingGearSwitchState == MIDSTATE) {
      // So it was LOSTATE, now open the hatch
      servoTargetPos[SERVOLANDINGHATCH] = SERVOLANDINGHATCHPOSOPEN;
      Serial.println("Open hatch");
    } else {
      // So it was in MIDSTATE, now extend the gear
      servoTargetPos[SERVOLANDINGGEAR] = SERVOLANDINGGEARPOSEXTENDED;
      Serial.println("Extend gear");
    }
  }
  landingGearSwitchStatePrev = landingGearSwitchState; //update the previous state for the next loop

}

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
    myServo[i].write(int(servoActualPos[i]));
    
  }

}