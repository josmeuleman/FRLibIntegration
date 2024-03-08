// Manager for a sensor on the analog input. It uses the SensorManager class, such that the Logger class can log the sensor.
// 
// 2023-03-19, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRAnalogInputManager_h
#define FRAnalogInputManager_h

#include <FRSensorManager.h>

//creates class which inherents Sensor
class AnalogInputManager : public SensorManager {
  public :
  AnalogInputManager();
  AnalogInputManager(int pinNumber);
  AnalogInputManager(int pinNumber, String headerString);
  ~AnalogInputManager();
  
  void SetPinNumber(int pinNumber);
  void SetHeaderString(String headerString);
  void SetOutputRange(float minValue, float maxValue);
  
  
  String HeaderString() override;
  String SensorString() override;

  private : 
  int _pinNumber;
  int _min, _max;
  const int _maxAD = 4095;
  String _headerString;
};

#endif