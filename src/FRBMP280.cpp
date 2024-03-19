// Wrapper for a BMP280 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-07, Jos Meuleman & Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands

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

void FRBMP280::AutoOffset(){
	_offsetPressure = _myBMP->readPressure()/100;
}

void FRBMP280::SetOffsetPressurehPa(float inPressure) {
	_offsetPressure = inPressure;
}


String FRBMP280::HeaderString(){
    String tempString;
    tempString.concat("Pressure; ");
    tempString.concat("Approx altitude; ");
    tempString.concat("Temperature; ");
    return tempString;
}

String FRBMP280::SensorString(){
    String tempString;

    tempString.concat(createFloatString(_myBMP->readPressure(), 2));
    tempString.concat(createFloatString(_myBMP->readAltitude(_offsetPressure), 2)); // Adjust for local pressure!!
    tempString.concat(createFloatString(_myBMP->readTemperature(), 2));

    return tempString;
}