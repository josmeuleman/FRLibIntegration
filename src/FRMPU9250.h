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
	
	void SetOffsetAcc(float ax0, float ay0, float az0);
	void AutoOffsetGyro();
	
	float GetAx() { return _myMPU->accel_x_mps2() - _ax0; }
	float GetAy() { return _myMPU->accel_y_mps2() - _ay0; }
	float GetAz() { return _myMPU->accel_z_mps2() - _az0; }

	float GetGx() { return _myMPU->gyro_x_radps() - _gx0; }
	float GetGy() { return _myMPU->gyro_y_radps() - _gy0; }
	float GetGz() { return _myMPU->gyro_z_radps() - _gz0; }

	float GetMx() { return _myMPU->mag_x_ut(); }
	float GetMy() { return _myMPU->mag_y_ut(); }
	float GetMz() { return _myMPU->mag_z_ut(); }

	
    String HeaderString() override;
    String SensorString() override;

    private :
    bfs::Mpu9250* _myMPU;
    const int MPU9250_ADDRESS = 0x68;
	double _ax0 = 0.0;
	float _ay0 = 0.0;
	float _az0 = 0.0;
	float _gx0 = 0.0;
	float _gy0 = 0.0;
	float _gz0 = 0.0;
	
	
};

#endif


