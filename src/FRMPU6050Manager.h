// Manager for a MPU6050 sensor. It uses the SensorManager class, such that the Logger class can log the sensor.
// 
// 2023-03-20, Jos Meuleman & Tim van Cuylenborg, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRMPU6050Manager_h
#define FRMPU6050Manager_h

#include <FRSensorManager.h>
#include <Adafruit_MPU6050.h>  // include the library for the MPU6050 sensor
#include <Wire.h>

//creates class which inherents Sensor
class MPU6050Manager : public SensorManager {
  public :
  MPU6050Manager();
  ~MPU6050Manager(); // Add a destructor
  bool Init(TwoWire &myWire, mpu6050_accel_range_t accelRange, mpu6050_gyro_range_t gyroRange);
  String HeaderString() override;
  String SensorString() override;

  private : 
  Adafruit_MPU6050* _myMPU; // Pointer to the sensor object
  const int MPU6050_ADDRESS = 0x68; // The I2C address of the MPU6050 sensor
};

#endif