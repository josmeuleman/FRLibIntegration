// Wrapper for a MPU9250 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-21, Jos Meuleman & Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRMPU9250.h"
#include "FRGeneric.h"

FRMPU9250::FRMPU9250() {
    _myMPU = new bfs::Mpu9250();
}

FRMPU9250::~FRMPU9250(){
    delete _myMPU;
}

bool FRMPU9250::Init(TwoWire &myWire) {
    _myMPU->Config(&myWire, bfs::Mpu9250::I2C_ADDR_PRIM);
    if (!_myMPU->Begin()) {
        // Serial.println("MPU9250 not found!");
        return false;
    } else {
        // Serial.println("MPU9250 found!");
		_myMPU->Read();
        return true;
    }
}

void FRMPU9250::SetOffsetAcc(float ax, float ay, float az){
	_ax0 = GetAx() - ax;
	_ay0 = GetAy() - ay;
	_az0 = GetAz() - az;
}

void FRMPU9250::AutoOffsetGyro(){
	_gx0 = _myMPU->gyro_x_radps();
	_gy0 = _myMPU->gyro_y_radps();
	_gz0 = _myMPU->gyro_z_radps();
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
    if (!_myMPU->Read()) {
        return "";
    }

    String tempString;
    
    tempString.concat(createFloatString(GetAx(), 3));
    tempString.concat(createFloatString(GetAy(), 3));
    tempString.concat(createFloatString(GetAz(), 3));
    tempString.concat(createFloatString(_myMPU->gyro_x_radps(), 3));
    tempString.concat(createFloatString(_myMPU->gyro_y_radps(), 3));
    tempString.concat(createFloatString(_myMPU->gyro_z_radps(), 3));
    tempString.concat(createFloatString(_myMPU->mag_x_ut(), 3));
    tempString.concat(createFloatString(_myMPU->mag_y_ut(), 3));
    tempString.concat(createFloatString(_myMPU->mag_z_ut(), 3));
    tempString.concat(createFloatString(_myMPU->die_temp_c(), 1));
    
    return tempString;
}
