// Wrapper for a MPU6050 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-07, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "FRTinyGPS.h"
#include "FRGeneric.h"

FRTinyGPS::FRTinyGPS(){
  _myGPS = new TinyGPSPlus();
}

FRTinyGPS::~FRTinyGPS(){
  delete _myGPS; 
}

bool FRTinyGPS::Init(){
  Serial2.begin(_BAUDGPS, SERIAL_8N1, _RX, _TX);
  return true;
}

bool FRTinyGPS::Init(float lat0Deg, float lon0Deg){
  Serial2.begin(_BAUDGPS, SERIAL_8N1, _RX, _TX);
  SetLat0(lat0Deg);
  SetLon0(lon0Deg);
  return true;
}

void FRTinyGPS::SetLat0( float lat0Deg ){
	_lat0Deg = lat0Deg;
	_cosLat0 = cos( _DEG2RAD * _lat0Deg);
}

void FRTinyGPS::SetLon0( float lon0Deg ){
	_lon0Deg = lon0Deg;
}

bool FRTinyGPS::HasValidData() {
  return(_myGPS->location.isValid());
}

String FRTinyGPS::HeaderString(){
  String tempString;
  tempString.concat("Satellites []; ");
  tempString.concat("Date [yyyy-mm-dd]; ");
  tempString.concat("Time [hh:mm:ss]; ");
  tempString.concat("Latitude [deg]; ");
  tempString.concat("Longitude [deg]; ");
  tempString.concat("Relative X [m]; ");
  tempString.concat("Relative Y [m]; ");
  tempString.concat("Altitude [m]; ");
  
  return tempString;
}

String FRTinyGPS::SensorString(){
  long tStart = millis();
  while ((Serial2.available() > 0) & (millis()-tStart < _TIMEOUTMS)){
    _myGPS->encode(Serial2.read());
  }	
  String tempString;
  tempString.concat(createIntString(GetSatellites()));
  if (_myGPS->date.isValid()) {
	tempString.concat(createDateString(_myGPS->date.year(), _myGPS->date.month(), _myGPS->date.day()));
  }
  else {
    tempString.concat(createDateString(9999, 99, 99));
  }
  
  if (_myGPS->time.isValid()) {
	tempString.concat(createTimeString(_myGPS->time.hour(), _myGPS->time.minute(), _myGPS->time.second()));
  }
  else {
    tempString.concat(createTimeString(99, 99, 99));
  }
  
  if (_myGPS->location.isValid()) {
	tempString.concat(createFloatString(GetLatitude(), 6));
	tempString.concat(createFloatString(GetLongitude(), 6));
	tempString.concat(createFloatString(GetRelativeX(), 6));
	tempString.concat(createFloatString(GetRelativeY(), 6));
  }
  else {
    tempString.concat(createFloatString(0, 6));
	tempString.concat(createFloatString(0, 6));  
    tempString.concat(createFloatString(0, 6));
	tempString.concat(createFloatString(0, 6));  
  }	  
  if (_myGPS->altitude.isValid()) {
	tempString.concat(createFloatString(GetAltitude(), 1));
  }
  else {
 	tempString.concat(createFloatString(0, 1));
  }

  return tempString;
}