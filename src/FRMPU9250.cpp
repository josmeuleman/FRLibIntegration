// Wrapper for a MPU9250 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-21, Jos Meuleman & Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRMPU9250.h"
#include "FRGeneric.h"
#include <MahonyAHRS.h>

FRMPU9250::FRMPU9250() {
    _myMPU = new bfs::Mpu9250();
	_myFilter = new Mahony();
}

FRMPU9250::~FRMPU9250(){
    delete _myMPU;
}

bool FRMPU9250::Init(TwoWire &myWire) {
    _myMPU->Config(&myWire, bfs::Mpu9250::I2C_ADDR_PRIM);
	_myFilter->begin(_sampleFrequencyHz);
	_myMPU->ConfigGyroRange(bfs::Mpu9250::GYRO_RANGE_250DPS);
	_myMPU->ConfigAccelRange(bfs::Mpu9250::ACCEL_RANGE_4G);
	
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
  tempString.concat("rotX [rad/s]; ");
  tempString.concat("rotY [rad/s]; ");
  tempString.concat("rotZ [rad/s]; ");
  tempString.concat("magX [uT]; ");
  tempString.concat("magY [uT]; ");
  tempString.concat("magZ [uT]; ");
  tempString.concat("roll [deg]; ");
  tempString.concat("pitch [deg]; ");
  tempString.concat("heading [deg]; ");
  tempString.concat("tempMPU [degC]; ");
  return tempString;
}

String FRMPU9250::SensorString() {
    if (!_myMPU->Read()) {
        
		//return "";
    }
	_myFilter->update(GetGx(), -GetGy(), -GetGz(), GetAx(), -GetAy(), -GetAz(), GetMx(), -GetMy(), -GetMz());
    String tempString;
    
    tempString.concat(createFloatString(GetAx(), 3));
    tempString.concat(createFloatString(GetAy(), 3));
    tempString.concat(createFloatString(GetAz(), 3));
    tempString.concat(createFloatString(GetGx(), 3));
    tempString.concat(createFloatString(GetGy(), 3));
    tempString.concat(createFloatString(GetGz(), 3));
    tempString.concat(createFloatString(GetMx(), 3));
    tempString.concat(createFloatString(GetMy(), 3));
    tempString.concat(createFloatString(GetMz(), 3));
	tempString.concat(createFloatString(GetRoll(), 1));
	tempString.concat(createFloatString(GetPitch(), 1));
	tempString.concat(createFloatString(GetHeading(), 1));
   
    tempString.concat(createFloatString(_myMPU->die_temp_c(), 1));
    
    return tempString;
}
