// Wrapper for a MPU9250 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-07, Jos Meuleman & Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRMPU9250_h
#define FRMPU9250_h

#include <FRSensor.h>
#include <mpu9250.h>
#include <Wire.h>

class FRMPU9250 : public FRSensor {
    public :
    FRMPU9250();
    ~FRMPU9250();
    bool Init(TwoWire &myWire);
    String HeaderString() override;
    String SensorString() override;

    private :
    bfs::Mpu9250* _myMPUv2;
    const int MPU9250_ADDRESS = 0x68;
};

#endif


