#ifndef CUSTOMFUNCTIONS_H
#define CUSTOMFUNCTIONS_H

//---------------------------------------------------------------------------------------------------------
// FUNCTIONS
// Here the custom functions are defined
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
// Function that prints a message to the serial port and OLED and updates the RGB LED
//---------------------------------------------------------------------------------------------------------
void Message(const String& _Message, RGBLED& _LED, rgbcolors _Color, HardwareSerial& _Serial, SSD1306AsciiWire& _OLED) {
  _LED.SetColor(_Color);
  _Serial.println(_Message);
  _OLED.println(_Message);
}

//---------------------------------------------------------------------------------------------------------
// Function that waits maximum x seconds. it returns true if within x seconds the button has been pushed
//---------------------------------------------------------------------------------------------------------
bool hasButtonBeenPressedDuringWait(int _WaitTimeSec, Button& inButton, HardwareSerial& _Serial, SSD1306AsciiWire& _OLED) {
  _Serial.printf("press the button for offset calibration within %d seconds, ", _WaitTimeSec);
  _Serial.println("else offset is not calibrated");

  // print a fancy bar to the Serial port . if the dots reach the end, the time is up
  byte lineLength = 20;
  for (byte i = 0; i <= lineLength; i++) {
    _Serial.print("-");
  }
  _Serial.println("|");

  _OLED.clear();
  _OLED.println("Press to calibrate");
  _OLED.setRow(1);

  // Start a counter
  long tStart = millis();
  float tCountDownS = float(_WaitTimeSec);
  bool isPressed = false;
  bool continueLoop = true;  // continue the loop until this is false
  while (continueLoop) {
    inButton.Update();
    if (inButton.HasChangedUp()) {  // button has been pushed, return true and quit the while loop
      isPressed = true;
      continueLoop = false;
    }
    tCountDownS = float(_WaitTimeSec) - (millis() - tStart) / 1000.0;
    if (tCountDownS <= 0.0) {
      continueLoop = false;
    }
    // Update Serial port by printing a dot
    delay(int(_WaitTimeSec * 1000 / lineLength));
    _Serial.print('.');  // running bar
    // update LED display by printing a dot
    _OLED.print('.');
  }

  _Serial.println();

  if (isPressed) {
    _Serial.println("Offsets will be calculated during setup");
    _OLED.clear();
    _OLED.println("Calibrating sensors");
  } else {
    _Serial.println("Default offset values will be used");
    _OLED.clear();
    _OLED.println("No calibration");
    _OLED.println("Using default values");
  }

  return isPressed;  // returns true if the button was pressed during this process, else false
}

//---------------------------------------------------------------------------------------------------------
// Function that calibrates the sensors. Add sensors at will
//---------------------------------------------------------------------------------------------------------
void CalibrateSensors(FRBMP280& _AltitudeSensor, FRMPU9250& _IMUSensor) {
  // Serial.println(_AltitudeSensor.HeaderString());
  // Serial.println(_AltitudeSensor.SensorString());
  _AltitudeSensor.AutoOffset();
  // Serial.println(_AltitudeSensor.SensorString());

  // Serial.println(_IMUSensor.HeaderString());
  // Serial.println(_IMUSensor.SensorString());
  _IMUSensor.AutoOffsetGyro();
  _IMUSensor.SetOffsetAcc(0.0, 0.0, -9.81);
  // Serial.println(_IMUSensor.SensorString());
}

//---------------------------------------------------------------------------------------------------------
// Function that checks the button and starts / stops the logger
//---------------------------------------------------------------------------------------------------------
void StartStopLogger(Button& button, RGBLED& _LED, Logger& logger, HardwareSerial& _Serial, SSD1306AsciiWire& _OLED) {
  myButton.Update();            // Read the state of the button
  if (button.HasChangedUp()) {  //Check if the state has changed from low to high
    _OLED.clear();
    if (!logger.IsLogging()) {  // It wasn't logging yet, so start logging
      _Serial.println("Start logging");
      if (!logger.StartLogger()) {
        Message("Something went wrong with the start of the log", _LED, RED, _Serial, _OLED);
      } else {  // the actual start of the logging
        Message(logger.GetLoggerFileName() + " opened", _LED, BLUE, _Serial, _OLED);
      }
    } else {  // Else we were logging, so now stop logging
      _Serial.println("Stop logging");
      if (!logger.StopLogger()) {
        Message("Something went wrong with the stopping of the log", _LED, RED, _Serial, _OLED);
      } else {
        Message("Logfile closed", _LED, GREEN, _Serial, _OLED);
      }
    }
  }
}
//---------------------------------------------------------------------------------------------------------
// Function that prints the GPS status to the OLED
//---------------------------------------------------------------------------------------------------------
void PrintGPSStatusToOLED(FRTinyGPS& _GPSSensor, SSD1306AsciiWire& _OLED) {
  _OLED.setRow(2);
  _OLED.print("# Sats found: ");
  _OLED.print(_GPSSensor.GetSatellites());
  _OLED.println("   ");  // some extra blank characters to overwrite old characters on that line
  if (_GPSSensor.HasValidData()) {
    _OLED.println("GPS fix    ");
  } else {
    _OLED.println("No GPS fix");
  }
}

#endif  //CUSTOMFUNCTIONS_H