// Wrapper for a MPU6050 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-07, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRTinyGPS_h
#define FRTinyGPS_h

#include <FRSensor.h>
#include <TinyGPSPlus.h>

//creates class which inherents Sensor
class FRTinyGPS : public FRSensor {
  public :
  FRTinyGPS();
  ~FRTinyGPS(); // Add a destructor
  bool Init();
  bool Init(float lat0Deg, float lon0Deg);
  
  void SetLat0( float lat0Deg );
  void SetLon0( float lon0Deg );
  
  float GetLatitude() { return _myGPS->location.lat(); }
  float GetLongitude() { return _myGPS->location.lng(); }
  float GetAltitude() { return _myGPS->altitude.meters(); }
  int GetSatellites() { return _myGPS->satellites.value(); }
  float GetRelativeX() { return _RADIUSEARTHM * _cosLat0 * sin( _DEG2RAD * (_myGPS->location.lng() - _lon0Deg) ); }
  float GetRelativeY() { return _RADIUSEARTHM * sin( _DEG2RAD * (_myGPS->location.lat() - _lat0Deg) ); }
  
  bool HasValidData();
  String HeaderString() override;
  String SensorString() override;

  private :
  const int _BAUDGPS = 9600;
  const int _TIMEOUTMS = 20;
  TinyGPSPlus* _myGPS;
  const long _RADIUSEARTHM = 6378100; // Average Radius Earth
  const float _DEG2RAD = 0.017453292519943295769236907684886;
  
  float _lat0Deg = 51.935751154805295; // location of model flying EMCR
  float _lon0Deg = 4.196820426972546;
  float _cosLat0 = cos( _DEG2RAD * _lat0Deg);
  

};

#endif