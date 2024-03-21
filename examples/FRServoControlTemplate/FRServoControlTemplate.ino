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
const byte PINSWITCH = 35;           // The pin number for he button to start and stop logging
const byte PINSERVO[NUMBEROFSERVOS] = {25, 26}; // Servo Channels
const byte PINPPM = 4;               // PM2 input pint

// other constants
const int  LOOPTIMESERVOMS = 100;     // Loop time for controlling servos


// Create all objects
Timer servoTimer(LOOPTIMESERVOMS);  // Timer object for the clock
Servo myServo[NUMBEROFSERVOS];      // create a servo object
Button myButton(PINSWITCH, true);   // Create a button object with the given pin. True for an inverted button, false for a normal button
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


const byte SERVOLANDINGGEAR = 0;        // the servo number of the landing gear
const byte SERVOLANDINGHATCH = 1;   // the servo number of the landing gear hatch
const int MAXSERVOSPEEDDEGS[NUMBEROFSERVOS] = {30, 30}; // Maximum speed of the servos in degrees per sec
int servoTargetPos[NUMBEROFSERVOS] = {0, 0}; //The initial state of the servos
float servoActualPos[NUMBEROFSERVOS];
const int SERVOLANDINGGEARPOSEXTENDED = 130;
const int SERVOLANDINGGEARPOSRETRACTED = 13;
const int SERVOLANDINGHATCHPOSOPEN = 27;
const int SERVOLANDINGHATCHPOSCLOSED = 176;



//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  myLed.SetColor(MAGENTA);
  Serial.begin(9600);  // Start the serial communciation
    
  myReceiver.Init();

  for (int i=0; i<NUMBEROFSERVOS; i++) {
    myServo[i].attach(PINSERVO[i]);
  }

  // Starting pos for servos. Make sure they are in the right positin
  servoTargetPos[SERVOLANDINGGEAR] = SERVOLANDINGGEARPOSEXTENDED;
  servoTargetPos[SERVOLANDINGHATCH] = SERVOLANDINGHATCHPOSOPEN;
  for (int i = 0; i < NUMBEROFSERVOS; i++) {
    servoActualPos[i] = servoTargetPos[i];
  }
  
  servoTimer.Start();
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
  myLed.SetColor(RED);
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