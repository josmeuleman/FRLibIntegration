#ifndef CUSTOMFUNCTIONS_H
#define CUSTOMFUNCTIONS_H

//---------------------------------------------------------------------------------------------------------
// CUSTOM FUNCTIONS
// Here the custom functions are defined
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
// (empty) Function that checks the logger switch and accordingly starts and stops the logger
//---------------------------------------------------------------------------------------------------------
void HandleLoggerSwitch(bool _SwitchState, bool _SwitchStatePrev){
  bool startLogger = (loggerSwitchState < loggerSwitchStatePrev);  //if new state is LOW (0) and old state is HIGH (1)
  bool stopLogger = (loggerSwitchState > loggerSwitchStatePrev);   //if new state is HIGH (1) and old state is LOW (1)
  if  (startLogger) {
    // insert code for starting logging
    Serial.println("Start logging");
  }
  if (stopLogger) {
    // insert code for stopping logging
    Serial.println("Stop logging ");
  }
}

//---------------------------------------------------------------------------------------------------------
// Function that check the landing gear switch and moves servos accordingly
//---------------------------------------------------------------------------------------------------------
void HandleMainLandingGearSwitch(triStateSwitch _SwitchState, triStateSwitch _SwitchStatePrev, SmoothServo& _ServoGear, SmoothServo& _ServoHatch){
  // landingGearSwitchState can have the state LOSTATE (-1), MIDSTATE (0), or HISTATE (1)
  // the function GetChannelTriState returns one of these three states depending on the value of the channel
  
  if (_SwitchState < _SwitchStatePrev) {
    // towards rectracted state
    if (_SwitchState == MIDSTATE) {
      // So it was HISTATE, now pull in the gear
      _ServoGear.SetTargetEnd();
      Serial.println("Retract gear");
    } else {
      // So it was in MIDSTATE, now close the hatch
      _ServoHatch.SetTargetEnd();
      Serial.println("Close hatch");
    }
  }
  if (_SwitchState > _SwitchStatePrev) {
    // towards extended state
    if (_SwitchState == MIDSTATE) {
      // So it was LOSTATE, now open the hatch
      _ServoHatch.SetTargetStart();
      Serial.println("Open hatch");
    } else {
      // So it was in MIDSTATE, now extend the gear
      _ServoGear.SetTargetStart();
      Serial.println("Extend gear");
    }
  }
}

//---------------------------------------------------------------------------------------------------------
// End of CUSTOM FUNCTIONS
//---------------------------------------------------------------------------------------------------------
#endif  //CUSTOMFUNCTIONS_H