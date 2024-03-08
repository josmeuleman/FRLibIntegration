// Wrapper for a ms4525do (differential pressure, pitot). It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-07, Jos Meuleman & Tim van Cuylenborg, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRMS4525DO_h
#define FRMS4525DO_h

#include <FRSensor.h>
#include <ms4525do.h>  // include the library for the ms4525do sensor
#include <Wire.h>

//creates class which inherents Sensor
class FRMS4525DO : public FRSensor {
  public :
  FRMS4525DO();
  virtual ~FRMS4525DO(); // Add a destructor
  bool Init(TwoWire &myWire);
  String HeaderString() override;
  String SensorString() override;

  private :
  bfs::Ms4525do* _myPitot; 
  const int PITOT_I2C_ADDRESS = 0x28;     // I2C address of the Pitot sensor
};

#endif