// Testscript for reading PPM signals on the Flight Recorder Board using the FRPPMReceiverManager
// The difference with FRPPMReceiver are:
//                FRPPMReceiverManager FRPPMReceiver
// Logger object    Yes                 No
// Update needed    No                  Yes
// Init needed      Yes                 No
// LowPassfiltering No                  Yes
// Output           int [600..1600]      float [-0.7..0.7]
//
// Required hardware:
// - 1x Turnigy TGY IA6B Receiver programmed in PPL mode
// - 1x Tunigy TGY-i6S Controller programmed in PPL mode
// - 3x breadboard cables female-male
// Connections:
// - TGY channel PPM, pin 1 - FR Board, PPL2 GND
// - TGY channel PPM, pin 2 - FR Board, PPL2, D4
// - TGY channel PPM, pin 3 - FR Board, SD card reader, 5V (or connected to other 5V source)
// Required libraries:
// - FRLib (download from https://github.com/josmeuleman/FRLib, unzipped in ../Documents/Arduino/libraries/ )
//
// 2023-04-17, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#include <FRPPMReceiverManager.h>
#include <FRLED.h>

const int PINLED = 21;  // The pin number for the LED
const int PINPPM = 4;   // PPL2 input pint
const int NUMBEROFCHANNELS = 4;
const int PRINTLOOP = 1;  //We print to the screen after X measurements

PPMReceiverManager MyReceiverManager(PINPPM, NUMBEROFCHANNELS);  // Create a PPM receiver object with given pin and number of channels

int channelValues[NUMBEROFCHANNELS];

//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  MyReceiverManager.Init();
}

//---------------------------------------------------------------------------------------------------------
// LOOP
// This block of code is looped infinitely
//---------------------------------------------------------------------------------------------------------
void loop() {
  for (int i = 0; i < NUMBEROFCHANNELS; i++) {  // Cycle through the channels to read and print the value
    channelValues[i] = MyReceiverManager.ReadChannel(i);
    Serial.print(channelValues[i]);
    Serial.print("; ");
  }
  Serial.println("");  // End of line
  delay(100);
}