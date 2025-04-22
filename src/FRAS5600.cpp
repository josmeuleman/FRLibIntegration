// Wrapper for a AS5600 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-15, Jos Meuleman & Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands
// 2025-04-22, Ruben Koningsveld, last update.

#include "FRAS5600.h"
#include "FRGeneric.h"

FRAS5600::FRAS5600(){
    _myAS5600 = new AS5600();
}

FRAS5600::~FRAS5600(){
    delete _myAS5600;
}

bool FRAS5600::Init(){
    if (!_myAS5600->begin()) {
        // Serial.println("AS5600 not found!");
        _isEnabled = false;
        return false;
    } else {
        // Serial.println("AS5600 found!");
        _isEnabled = true;
        return true;
    }
}

bool FRAS5600::Init(float offsetAngle){
    this->SetOffsetAngle(offsetAngle);
    if (!_myAS5600->begin()) {
        // Serial.println("AS5600 not found!");
        _isEnabled = false;
        return false;
    } else {
        // Serial.println("AS5600 found!");
        _isEnabled = true;
        return true;
    }
}


String FRAS5600::HeaderString(){
    String tempString;
    tempString.concat("Angle [deg]; ");
    return tempString;
}

String FRAS5600::SensorString(){
    String tempString;
    if (!_isEnabled) {
        tempString.concat("NAN; ");
    }
    else {
        tempString.concat(createFloatString(this->GetAngle(), 2));    
    }
    return tempString;
}
