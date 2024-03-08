// Wrapper for a MPU9250 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-07, Jos Meuleman & Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRMPU9250.h"
#include "FRGeneric.h"

FRMPU9250::FRMPU9250() {
    _myMPUv2 = new bfs::Mpu9250();
}

FRMPU9250::~FRMPU9250(){
    delete _myMPUv2;
}

bool FRMPU9250::Init(TwoWire &myWire) {
    _myMPUv2->Config(&myWire, bfs::Mpu9250::I2C_ADDR_PRIM);
    if (!_myMPUv2->Begin()) {
        // Serial.println("MPU9250 not found!");
        return false;
    } else {
        // Serial.println("MPU9250 found!");
        return true;
    }
}

String FRMPU9250::HeaderString(){

  String tempString;
  tempString.concat("accX [m/s2]; ");
  tempString.concat("accY [m/s2]; ");
  tempString.concat("accZ [m/s2]; ");
  tempString.concat("rotX [deg/s]; ");
  tempString.concat("rotY [deg/s]; ");
  tempString.concat("rotZ [deg/s]; ");
  tempString.concat("magX [uT]; ");
  tempString.concat("magY [uT]; ");
  tempString.concat("magZ [uT]; ");
  tempString.concat("tempMPU [degC]; ");
  return tempString;
}

String FRMPU9250::SensorString() {
    if (!_myMPUv2->Read()) {
        return "";
    }

    String tempString;
    
    tempString.concat(createFloatString(_myMPUv2->accel_x_mps2(), 3));
    tempString.concat(createFloatString(_myMPUv2->accel_y_mps2(), 3));
    tempString.concat(createFloatString(_myMPUv2->accel_z_mps2(), 3));
    tempString.concat(createFloatString(_myMPUv2->gyro_x_radps(), 3));
    tempString.concat(createFloatString(_myMPUv2->gyro_y_radps(), 3));
    tempString.concat(createFloatString(_myMPUv2->gyro_z_radps(), 3));
    tempString.concat(createFloatString(_myMPUv2->mag_x_ut(), 3));
    tempString.concat(createFloatString(_myMPUv2->mag_y_ut(), 3));
    tempString.concat(createFloatString(_myMPUv2->mag_z_ut(), 3));
    tempString.concat(createFloatString(_myMPUv2->die_temp_c(), 1));
    
    return tempString;
}
