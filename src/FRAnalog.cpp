// Manager for a sensor on the analog input. It uses the SensorManager class, such that the Logger class can log the sensor.
// 
// 2024-03-21, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRAnalog.h"
#include "FRGeneric.h"
#include <Arduino.h>

FRAnalog::FRAnalog(){
  _pinNumber = -1;
  _headerString = "";
  _min = 0;
  _max = _MAXAD;
}

FRAnalog::FRAnalog(byte pinNumber){
  _pinNumber = pinNumber;
  _headerString = "";
  _min = 0;
  _max = _MAXAD;
  pinMode(_pinNumber, INPUT);
}

FRAnalog::FRAnalog(byte pinNumber, String headerString) {
  _pinNumber = pinNumber;
  _headerString = headerString;
  _min = 0;
  _max = _MAXAD;
  pinMode(_pinNumber, INPUT);  
}
  
void FRAnalog::SetPinNumber(byte pinNumber){
  _pinNumber = pinNumber;
  _headerString = "";
  _min = 0;
  _max = _MAXAD;
  pinMode(_pinNumber, INPUT);    
}

void FRAnalog::SetHeaderString(String headerString){
  _headerString = headerString;  
}

FRAnalog::~FRAnalog(){

}

void FRAnalog::SetOutputRange(float minValue, float maxValue) {
  _min = minValue;
  _max = maxValue;  

}

String FRAnalog::HeaderString(){
  String tempString = _headerString;
  tempString.concat("; ");
  return tempString;
}

String FRAnalog::SensorString(){
  int val = analogRead(_pinNumber);
  String tempString;
  if ((_min==0) && (_max==_MAXAD)) {
	tempString = createIntString(val);
  }
  else {
	float valf = mapf(val, 0, _MAXAD, _min, _max);
	tempString = createFloatString(valf,2);
  }	
  return tempString;
}