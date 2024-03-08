// Demo for communication with a PixHawk Pitot sensor with Flight Recorder PCB
// Required hardware:
// - 1x Flight Recorder Board with ESP32
// - 1x Pitot Sensor (tested on PX4AirspeedV1.1)
// Connections:
// - J9 (screw terminal), 3.3V - PixHawk, 5V (yes this will work)
// - J9 (screw terminal), SCL - PixHawk, SCL
// - J9 (screw terminal), SDA - PixHawk, SDA
// - J9 (screw terminal), GND - PixHawk, GND
// Required libraries:
// - FRLib (download from https://github.com/josmeuleman/FRLib, unzipped in ../Documents/Arduino/libraries/ )
// - ms4525do
//
// 2023-04-05, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#include <ms4525do.h> 

const int I2C_SDA = 33;                 // The data pin for I2C communication
const int I2C_SCL = 32;                 // The clock pin for I2C communcation
const int PITOT_I2C_ADDRESS = 0x28;     // I2C address of the Pitot sensor

const int BAUDSERIAL = 9600;  //baud rate for serial communication

bfs::Ms4525do MyPitot; // Make an object for the sensor
String PitotString; // String for the data of the pitot sensor for writing, plotting or logging

//---------------------------------------------------------------------------------------------------------
// SETUP
// This block of code is only run once at the beginning
//---------------------------------------------------------------------------------------------------------
void setup() {
  // Start the Serial communication to your computer. Make sure the baud rate of the monitor is equal to the baud rate as set here
  Serial.begin(BAUDSERIAL);

  // Set up the communication for I2C sensors
  Wire.begin(I2C_SDA, I2C_SCL);
  Wire.setClock(400000);
  //I2C address for Pitot Sensor is 0x28, on bus 0, with a -1 to +1 PSI range
  MyPitot.Config(&Wire, PITOT_I2C_ADDRESS, 1.0f, -1.0f);
  // Start communication with the Pitot Sensor. If this fails, the program will stop here
  if (!MyPitot.Begin()) {
    Serial.println("Error communicating with pitot sensor");
    return;
  }

}

//---------------------------------------------------------------------------------------------------------
// LOOP
// This block of code is looped infinitely
//---------------------------------------------------------------------------------------------------------
void loop() {
  // read the pitot sensor. It will return true, if successfully read.
  if (MyPitot.Read()) {
    //PitotString = makePitotLongString(MyPitot); // Make a long string, with extra text, convenient for in the serial monitor
    PitotString = makePitotShortString(MyPitot); // Make a short string with just the data, convenient for plotting and logging
    Serial.println(PitotString);
  }
  delay(100);  // wait 100 milliseconds before going back to the beginning
}

//---------------------------------------------------------------------------------------------------------
// FUNCTIONS & METHODS
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
// Generate a readable string for the Pitot sensor
//---------------------------------------------------------------------------------------------------------
String makePitotLongString(bfs::Ms4525do MyPitot){
  String myString ="";
  myString+= "Pressure [Pa]: ";
  myString+= MyPitot.pres_pa();
  myString+= ", Pitot Temp [C]: ";
  myString+= MyPitot.die_temp_c();
  return myString;
}

//---------------------------------------------------------------------------------------------------------
// Generate a string with only data for the Pitot sensor
//---------------------------------------------------------------------------------------------------------
String makePitotShortString(bfs::Ms4525do MyPitot){
  String myString ="";
  myString+= MyPitot.pres_pa();
  myString+= "; ";
  myString+= MyPitot.die_temp_c();
  myString+= "; ";
  return myString;
}
