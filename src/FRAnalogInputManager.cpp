// Manager for a sensor on the analog input. It uses the SensorManager class, such that the Logger class can log the sensor.
// 
// 2023-03-19, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRAnalogInputManager.h"
#include "FRGeneric.h"
#include <Arduino.h>

AnalogInputManager::AnalogInputManager(){
  _pinNumber = -1;
  _headerString = "";
  _min = 0;
  _max = _maxAD;
}

AnalogInputManager::AnalogInputManager(int pinNumber){
  _pinNumber = pinNumber;
  _headerString = "";
  _min = 0;
  _max = _maxAD;
  pinMode(_pinNumber, INPUT);
}

AnalogInputManager::AnalogInputManager(int pinNumber, String headerString) {
  _pinNumber = pinNumber;
  _headerString = headerString;
  _min = 0;
  _max = _maxAD;
  pinMode(_pinNumber, INPUT);  
}
  
void AnalogInputManager::SetPinNumber(int pinNumber){
  _pinNumber = pinNumber;
  _headerString = "";
  _min = 0;
  _max = _maxAD;
  pinMode(_pinNumber, INPUT);    
}

void AnalogInputManager::SetHeaderString(String headerString){
  _headerString = headerString;  
}

AnalogInputManager::~AnalogInputManager(){

}

void AnalogInputManager::SetOutputRange(float minValue, float maxValue) {
  _min = minValue;
  _max = maxValue;  

}

String AnalogInputManager::HeaderString(){
  String tempString = _headerString;
  tempString.concat("; ");
  return tempString;
}

String AnalogInputManager::SensorString(){
  int val = analogRead(_pinNumber);
  String tempString;
  if ((_min==0) && (_max==_maxAD)) {
	tempString = createIntString(val);
  }
  else {
	float valf = mapf(val, 0, _maxAD, _min, _max);
	tempString = createFloatString(valf,2);
  }	
  return tempString;
}