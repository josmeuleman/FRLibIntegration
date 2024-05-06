// Test script to see if the EPS can handle two controllers
// Required hardware:
// - 1x Flight Recorder Board v2 with ESP32
// - 2x Turnigy TGY IA6B Receiver programmed in PPM mode
// - 2x Tunigy TGY-i6S Controller programmed in PPM mode
// - 1x OLED connected to board
// Connections:
// - TGY1 channel PPM, pin 1 - FR Board, J3-GND
// - TGY1 channel PPM, pin 2 - FR Board, J3-+5V
// - TGY1 channel PPM, pin 3 - FR Board, J3-D4
// - TGY2 channel PPM, pin 1 - FR Board, J2-GND
// - TGY2 channel PPM, pin 2 - FR Board, J2-+5V
// - TGY2 channel PPM, pin 3 - FR Board, J2-D2
// Required libraries:
// - FRLibBasics (download from https://github.com/josmeuleman/FRLibBasics, unzipped in ../Documents/Arduino/libraries/ )
// - FRLibIntegration (download from https://github.com/josmeuleman/FRLibIntegration, unzipped in ../Documents/Arduino/libraries/ )
//
// 2024-05-06, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

// Libraries from FRLibBasics
#include <FRTimer.h>

// Libraries from FRLibIntegration
#include <FRPPMReceiver.h>

// Other libraries
#include <SSD1306Ascii.h>      //i2C OLED
#include <SSD1306AsciiWire.h>  //i2C OLED

const byte NUMBEROFCHANNELS = 4;  // Number of Channels for both receivers

// Pins
const byte I2C_SDA = 21;     // The data pin for I2C communication
const byte I2C_SCL = 22;     // The clock pin for I2C communcation
const byte PINPPM1 = 4;      // PM2 input pint
const byte PINPPM2 = 2;      // Use of Pin 2 will turn on the internal blue LED. Use pin 15 if you want to use the internal led (LED_BUILTIN)


// other constants
const int LOGGERLOOPTIMEMS = 100;  // Loop time for logging

const int OLED_ADDRESS = 0x3C;  // I2C address of OLED

// Create all objects
Timer myTimer(LOGGERLOOPTIMEMS);  // Timer object for the clock
FRPPMReceiver myReceiver1(PINPPM1, NUMBEROFCHANNELS);  // Create a PPM receiver object with given pin and number of channels
FRPPMReceiver myReceiver2(PINPPM2, NUMBEROFCHANNELS);  // Create a PPM receiver object with given pin and number of channels
SSD1306AsciiWire myOLED;

int channelsValues1[NUMBEROFCHANNELS];
int channelsValues2[NUMBEROFCHANNELS];

//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  // Start the serial communication for all I2C sensors
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);

  myOLED.begin(&Adafruit128x32, OLED_ADDRESS);
  myOLED.setFont(Adafruit5x7);
  myOLED.clear();  //clear display
  
  Serial.begin(9600);
  delay(1000);  // safe to wait a few milliseconds after serial.begin before writing to serial port
  Serial.println("Setup ");

  myReceiver1.Init();
  myReceiver2.Init();


  myTimer.Start();
  Serial.println("End of Setup");
  myOLED.clear();  //clear display

}

//---------------------------------------------------------------------------------------------------------
// LOOP
// This block of code is looped infinitely
//---------------------------------------------------------------------------------------------------------
void loop() {
  //-------------------------------------------------------------------------------------------------------
  // Read the myReceiver1 and store the values
  //-------------------------------------------------------------------------------------------------------
  for (byte i = 0; i < NUMBEROFCHANNELS; i++) {
    channelsValues1[i] = myReceiver1.ReadChannel(i);
    Serial.print(channelsValues1[i]);
    Serial.print("; ");
  }
  for (byte i = 0; i < NUMBEROFCHANNELS; i++) {
    channelsValues2[i] = myReceiver2.ReadChannel(i);
    Serial.print(channelsValues2[i]);
    Serial.print("; ");
  }
  Serial.println();

  myOLED.clear();  //clear display
  myOLED.println(channelsValues1[0]);
  myOLED.println(channelsValues2[0]);

  //-------------------------------------------------------------------------------------------------------
  // End of the loop
  //-------------------------------------------------------------------------------------------------------
  // At the end of the loop, WaitUntilEnd runs until the time until looptime has passed
  if (myTimer.WaitUntilEnd()) {
    Serial.println("Overrun!");  // if there are delays in the loop, you will get overruns i.e. the loop took longer than the looptime
  }
  //
}
