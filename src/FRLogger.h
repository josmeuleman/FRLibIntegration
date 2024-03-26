// Class for logging data to an SD card 
// 
// 2024-03-07, Jos Meuleman & Tim van Cuylenborg, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRLogger_h
#define FRLogger_h

#include <SD.h>  // include the library for the SD card
#include <FS.h>  // include the library for writing files
#include <FRSensor.h> //has access to the virtual functions of the sensor class
#include <vector>


// typedef enum {
//   MPU6050,
//   TINYGPS,
//   MS4525DO
// } logger_sensortype;

class Logger{
  public:
  Logger();
  bool CheckSD();
  void AddSensor(FRSensor* Sensor);
  String GetLoggerFileName();
  
  String GetHeaderString() { return _headerString; }
  String GetSensorString() { return _loggerString; }

  bool IsLogging();
  bool StartLogger();
  bool StopLogger();
  String UpdateSensors();
  void WriteLogger();
  
  // void AddFloatToLogLine(float value, String format);
  // void AddStringToLogLine(float value, String format);


  private:
  String FindNewFileName();
  std::vector<FRSensor*> sensors;  // vector of Sensor objects
  bool _isLogging;
  String _fileName;
  File _file;
  String _loggerString;
  String _headerString;
  
  
};


#endif