// Wrapper for a ms4525do (differential pressure, pitot). It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-07, Jos Meuleman & Tim van Cuylenborg, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRMS4525DO.h"
#include "FRGeneric.h"

FRMS4525DO::FRMS4525DO() {
  _myPitot = new bfs::Ms4525do(); // Allocate memory for the object
}

FRMS4525DO::~FRMS4525DO() {
  //_myPitot.~Ms4525do(); // Call the destructor of the object
  delete _myPitot;
}

bool FRMS4525DO::Init(TwoWire &myWire){
  _myPitot->Config(&Wire, PITOT_I2C_ADDRESS, 1.0f, -1.0f);
  // Start communication with the Pitot Sensor. If this fails, the program will stop here
  if (!_myPitot->Begin()) {
    //Serial.println("Error communicating with ms4525do");
    return false;
  }
  else {
    //Serial.println("ms4525do found!");
	return true;
  }
}

String FRMS4525DO::HeaderString(){
  String tempString;
  tempString.concat("pressure [Pa]; ");
  tempString.concat("tempPitot [degC]; ");
  return tempString;
}

String FRMS4525DO::SensorString(){
  _myPitot->Read();
  String tempString;
  
  tempString.concat(createFloatString(_myPitot->pres_pa(), 3));
  tempString.concat(createFloatString(_myPitot->die_temp_c(), 1));
  
  return tempString;
}