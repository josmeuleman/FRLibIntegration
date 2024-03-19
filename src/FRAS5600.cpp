// Wrapper for a AS5600 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-15, Jos Meuleman & Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRAS5600.h"
#include "FRGeneric.h"

FRAS5600::FRAS5600(){
    _myAS5600 = new FRAS5600();
}

FRAS5600::~FRAS5600(){
    delete _myAS5600;
}

bool FRAS5600::Init(){
    return _myAS5600->begin();
}

String FRAS5600::HeaderString(){
    String tempString;
    tempString.concat("Angle; ");
    return tempString;
}

String FRAS5600::SensorString(){
    String tempString;
    tempString.concat(createFloatString(_myAS5600->readAngle(), 2));
    return tempString;
}
