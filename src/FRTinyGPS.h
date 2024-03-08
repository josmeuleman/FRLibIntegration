// Wrapper for a MPU6050 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-07, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRTinyGPS_h
#define FRTinyGPS_h

#include <FRSensor.h>
#include <TinyGPSPlus.h>

//creates class which inherents Sensor
class FRTinyGPS : public FRSensor {
  public :
  FRTinyGPS();
  ~FRTinyGPS(); // Add a destructor
  bool Init();
  bool HasValidData();
  String HeaderString() override;
  String SensorString() override;

  private :
  const int _BAUDGPS = 9600;
  const int _TIMEOUTMS = 20;
  TinyGPSPlus* _myGPS;

};

#endif