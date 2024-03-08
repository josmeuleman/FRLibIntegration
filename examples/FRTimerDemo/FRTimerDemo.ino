// Demoscript for Timer for  Flight Recorder PCB
// Required hardware:
// - 1x GPS Sensor + Antenna (GY-GPSV3-NEO)
// Connections:
// - GPS mounted on board
// Required libraries:
// - FRLib (download from https://github.com/josmeuleman/FRLib, unzipped in ../Documents/Arduino/libraries/ )
// - TinyGPSPlus
//
// 2023-03-16, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include <FRGeneric.h>
#include <FRTimer.h>

HardwareSerial SerialGPS(2);  // The serial connection to the GPS device
TinyGPSPlus gps;
Timer myTimer(1000); // Create a timer with 1000 millisecond cycle

//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);     // Start the serial communciation
  SerialGPS.begin(9600);
  myTimer.Start();
}

//---------------------------------------------------------------------------------------------------------
// LOOP
// This block of code is looped infinitely
//--------------------------------------------------------------------------------------------------------
void loop() {
  long tMillis = millis();

  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }
  Serial.printf("The time since startup in milliseconds: %d; ", tMillis);
  Serial.printf("The time of the GPS sensor: %s",createTimeString(gps.time.hour(), gps.time.minute(), gps.time.second()));
  
  Serial.println();
  myTimer.WaitUntilEnd();
}