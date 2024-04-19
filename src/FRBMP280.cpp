// Wrapper for a BMP280 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-21, Jos Meuleman & Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRBMP280.h"
#include "FRGeneric.h"

FRBMP280::FRBMP280(){
    _myBMP = new Adafruit_BMP280();
}

FRBMP280::~FRBMP280(){
    delete _myBMP;
}

bool FRBMP280::Init(TwoWire &myWire){
    if (!_myBMP->begin(BMP280_ADDRESS_ALT)) {
        return false;
    }
    return true;
}

String FRBMP280::HeaderString(){
    String tempString;
    tempString.concat("Pressure [Pa]; ");
    tempString.concat("Approx altitude [m]; ");
    tempString.concat("Temperature [degC]; ");
    return tempString;
}

String FRBMP280::SensorString(){
    String tempString;
    tempString.concat( createFloatString( GetPressure(), 0) );
    tempString.concat( createFloatString( GetAltitude(), 2) ); // Adjust for local pressure!!
    tempString.concat( createFloatString( _myBMP->readTemperature(), 1) );
    return tempString;
}