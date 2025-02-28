#ifndef CUSTOMFUNCTIONS_H
#define CUSTOMFUNCTIONS_H

//---------------------------------------------------------------------------------------------------------
// CUSTOM FUNCTIONS
// Here the custom functions are defined
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
// Function that check the landing gear switch and moves servos accordingly
//---------------------------------------------------------------------------------------------------------
void HandleMainLandingGearSwitch(triStateSwitch _SwitchState, triStateSwitch _SwitchStatePrev){
  // landingGearSwitchState can have the state LOSTATE (-1), MIDSTATE (0), or HISTATE (1)
  // the function GetChannelTriState returns one of these three states depending on the value of the channel
  landingGearSwitchState = myReceiver.GetChannelTriState(LANDINGGEARCHANNEL);

  if (_SwitchState < _SwitchStatePrev) {
    // towards rectracted state
    if (_SwitchState == MIDSTATE) {
      // So it was HISTATE, now pull in the gear
      //servoTargetPos[SERVOLANDINGGEAR] = SERVOLANDINGGEARPOSRETRACTED;
      Serial.println("Retract gear");
    } else {
      // So it was in MIDSTATE, now close the hatch
      //servoTargetPos[SERVOLANDINGHATCH] = SERVOLANDINGHATCHPOSCLOSED;
      Serial.println("Close hatch");
    }
  }
  if (_SwitchState > _SwitchStatePrev) {
    // towards extended state
    if (_SwitchState == MIDSTATE) {
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
// End of CUSTOM FUNCTIONS
//---------------------------------------------------------------------------------------------------------
#endif  //CUSTOMFUNCTIONS_H