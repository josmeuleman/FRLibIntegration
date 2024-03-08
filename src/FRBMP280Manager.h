// Wrapper for a BMP280 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-07, Jos Meuleman & Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRBMP280_h
#define FRBMP280_h

#include <FRSensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

class FRBMP280 : public FRSensor {
    public:
        FRBMP280();
        ~FRBMP280();
        bool Init(TwoWire &myWire);
        String HeaderString() override;
        String SensorString() override;

    private:
        Adafruit_BMP280* _myBMP;
        static const int MY_BMP280_ADDRESS = 0x76;
};

#endif