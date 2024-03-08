// Manager for a MPU6050 sensor. It uses the SensorManager class, such that the Logger class can log the sensor.
// 
// 2023-03-20, Jos Meuleman & Tim van Cuylenborg, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRMPU6050Manager.h"
#include "FRGeneric.h"

MPU6050Manager::MPU6050Manager(){
  _myMPU = new Adafruit_MPU6050(); // Allocate memory for the object
}


MPU6050Manager::~MPU6050Manager(){
  delete _myMPU; // Free the memory for the object
}

bool MPU6050Manager::Init(TwoWire &myWire, mpu6050_accel_range_t accelRange, mpu6050_gyro_range_t gyroRange){

  if (!_myMPU->begin(MPU6050_ADDRESS, &myWire)) {
    //Serial.println("Error communicating with MPU6050");
    return false;
  }
  else {
    //Serial.println("MPU6050 found!");
	_myMPU->setAccelerometerRange(accelRange);
	_myMPU->setGyroRange(gyroRange);
	return true;
  }
}

String MPU6050Manager::HeaderString(){
  String tempString;
  tempString.concat("accX [m/s2]; ");
  tempString.concat("accY [m/s2]; ");
  tempString.concat("accZ [m/s2]; ");
  tempString.concat("rotX [deg/s]; ");
  tempString.concat("rotY [deg/s]; ");
  tempString.concat("rotZ [deg/s]; ");
  tempString.concat("tempMPU [degC]; ");
  return tempString;
}

String MPU6050Manager::SensorString(){
  sensors_event_t a, g, temp;
  _myMPU->getEvent(&a, &g, &temp);
  String tempString;
  
  tempString.concat(createFloatString(a.acceleration.x, 3));
  tempString.concat(createFloatString(a.acceleration.y, 3));
  tempString.concat(createFloatString(a.acceleration.z, 3));
  tempString.concat(createFloatString(g.gyro.x, 3));
  tempString.concat(createFloatString(g.gyro.y, 3));
  tempString.concat(createFloatString(g.gyro.z, 3));
  tempString.concat(createFloatString(temp.temperature,1));
 
  return tempString;
}