// Wrapper for a AS5600 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-15, Jos Meuleman & Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRAS5600.h"
#include "FRGeneric.h"

FRAS5600::FRAS5600(){
    _myAS5600 = new AS5600();
}

FRAS5600::~FRAS5600(){
    delete _myAS5600;
}

bool FRAS5600::Init(){
    return _myAS5600->begin();
}

bool FRAS5600::Init(float offsetAngle){
	bool success = _myAS5600->begin();
	this->SetOffsetAngle(offsetAngle);
	return success;
}

void FRAS5600::SetOffsetAngle(float offsetAngle) {
	_myAS5600->setOffset(-offsetAngle);
}

String FRAS5600::HeaderString(){
    String tempString;
    tempString.concat("Angle; ");
    return tempString;
}

String FRAS5600::SensorString(){
    String tempString;
    tempString.concat(createFloatString(_myAS5600->readAngle()*AS5600_RAW_TO_DEGREES, 2));
    return tempString;
}
