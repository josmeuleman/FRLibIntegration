// Manager for a AS5600 sensor. It uses the SensorManager class, such that the Logger class can log the sensor.
// 
// 2024-01-23, Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands


#ifndef FRAS5600Manager_h
#define FRAS5600Manager_h

#include <FRSensorManager.h>
#include <AS5600.h>
#include <Wire.h>

class AS5600Manager : public SensorManager {
public:
    AS5600Manager();
    ~AS5600Manager();
    bool Init();
    String HeaderString() override;
    String SensorString() override;

private:
    AS5600* _myAS5600;
};

#endif