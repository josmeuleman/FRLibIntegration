// Testscript for reading testing GPS raw data on the Flight Recorder Board
// Required hardware:
// - 1x GPS Sensor + Antenna (GY-GPSV3-NEO)
// Connections:
// - GPS mounted on board
// Required libraries:
// - FRLib (download from https://github.com/josmeuleman/FRLib, unzipped in ../Documents/Arduino/libraries/ )
// - TinyGPSPlus
// Note:
// With this script, only raw information from the GPS is plotted
// For GPS time, position and speed, more programming is needed
// The raw message, when the GPS makes connection, when no satellites are found, looks like:
// $GNRMC,,V,,,,,,,,,,N*4D
// $GNVTG,,,,,,,,,N*2E
// $GNGGA,,,,,,0,00,99.99,,,,,,*56
// $GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E
// $GNGSA,A,1,,,,,,,,,,,,,99.99,99.99,99.99*2E
// $GPGSV,1,1,00*79
// $GLGSV,1,1,00*65
// $GNGLL,,,,,,V,N*7A
// Message length: 225
//
// 2023-03-09, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands


#include <HardwareSerial.h>

HardwareSerial SerialGPS(2); // This makes a connection to the second Serial port, to which the GPS is connected

long tlast; // variable for time
String msg; // variable for a text message from the GPS sensor

//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  SerialGPS.begin(9600);
}

//---------------------------------------------------------------------------------------------------------
// LOOP
// This block of code is looped infinitely
//---------------------------------------------------------------------------------------------------------
void loop() {
  while (SerialGPS.available() > 0) { // As long as the GPS sends data to the board...
    msg += (char)SerialGPS.read(); // convert that number to a character, and add it to msg
    tlast = millis(); // record the time
  }
  if ((millis() - tlast) > 10) { // if there is no GPS data received for 10 milliseconds...
    if (msg.length() > 0) { // and if the message is longer than 0 ...
      Serial.print(msg); // print the message
      Serial.printf("Message length: %d\n", msg.length()); // And print the message length
      Serial.println();
      msg = ""; // clear the message
    }
  }
}
