// Testscript for checking communication with a MPU6050 accelerometer & gyro on Flight Recorder Board
// Required hardware:
// - 1x Flight Recorder Board with ESP32
// - 1x MPU6050
// Connections:
// 
// Required libraries:
// - FRLib (download from https://github.com/josmeuleman/FRLib, unzipped in ../Documents/Arduino/libraries/ )
// - Adafruit_MPU6050
//
// 2023-03-04, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#include <Adafruit_MPU6050.h>  // include the library for the MPU6050 sensor

const int I2C_SDA = 33; // The data pin for I2C communication
const int I2C_SCL = 32; // The clock pin for I2C communcation
const int MPU6050_ADDRESS = 0x68; // The I2C address of the MPU6050 sensor

const int BAUDSERIAL = 9600;  //baud rate for serial communication

Adafruit_MPU6050 myMPU; // Make an object for the sensor

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
  // Start communication with the MPU Sensor. If this fails, the program will stop here
  if (!myMPU.begin(MPU6050_ADDRESS, &Wire)) {
    Serial.println("Error communicating with MPU6050");
    while (true) {};
  }
  else {
    Serial.println("MPU6050 found!");
  }
}

//---------------------------------------------------------------------------------------------------------
// LOOP
// This block of code is looped infinitely
//---------------------------------------------------------------------------------------------------------
void loop() {
}

