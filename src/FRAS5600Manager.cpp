// Manager for a AS5600 sensor. It uses the SensorManager class, such that the Logger class can log the sensor.
// 
// 2024-01-24, Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRAS5600Manager.h"
#include "FRGeneric.h"

AS5600Manager::AS5600Manager(){
    _myAS5600 = new AS5600();
}

AS5600Manager::~AS5600Manager(){
    delete _myAS5600;
}

bool AS5600Manager::Init(){
    return _myAS5600->begin();
}

String AS5600Manager::HeaderString(){
    String tempString;
    tempString.concat("Angle; ");
    return tempString;
}

String AS5600Manager::SensorString(){
    String tempString;
    tempString.concat(createFloatString(_myAS5600->readAngle(), 2));
    return tempString;
}
