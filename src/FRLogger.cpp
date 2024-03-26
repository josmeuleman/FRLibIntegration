// Class for logging data to an SD card 
// 
// 2024-03-07, Jos Meuleman & Tim van Cuylenborg, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "Arduino.h"
#include "FRLogger.h"

Logger::Logger() {
  _isLogging = false;
  _fileName = "";
  _loggerString.reserve(512);
  _headerString.clear();
  _headerString.concat("Time[ms]; "); 
}

bool Logger::CheckSD() {
  return (SD.begin());
}

void Logger::AddSensor(FRSensor *Sensor){
  sensors.push_back(Sensor);
  _headerString.concat(Sensor->HeaderString());
}

String Logger::GetLoggerFileName() {
  return _fileName;
}

bool Logger::IsLogging() {
  return _isLogging;
}

bool Logger::StartLogger() {
  if (_isLogging) {  // Already logging
    return false;
  }

  _fileName = FindNewFileName();
  if (_fileName.length() == 0) {  // No proper filename found
    return false;
  }

  _file = SD.open(_fileName, FILE_WRITE);
  if (!_file) {  //Failed to open the file
    return false;
  }
  
  // Successfully opened a logfile. Write the header line

  //for (int i = 0; i < sensors.size(); i++) {
  //  _headerString.concat(sensors[i]->HeaderString());
  //}
  _file.println(_headerString);

  _isLogging = true;
  return true;
}

bool Logger::StopLogger() {
  if (!_isLogging) { // Logger not running
    return false;
  }

  if (!_file) { // File not open
    return false;
  }

  _file.close();
  _isLogging = false;
  return true;
}


String Logger::UpdateSensors(){
  _loggerString = "";
  _loggerString.concat(String(millis())); // Add the time
  _loggerString.concat("; ");
  for (int i = 0; i < sensors.size(); i++) { // Loop through the sensors
	_loggerString.concat(sensors[i]->SensorString());
  }
  _loggerString.concat("\n"); // close with a new line
  return _loggerString;
}

void Logger::WriteLogger(){
  if (_isLogging){
    _file.print(_loggerString);
  }
}

String Logger::FindNewFileName() {
  int fileNumber = 1;
  while (fileNumber <= 99999) {
    _fileName = "/LOG";
    String fileNumberString = String(fileNumber);
    for (int i = 0; i < 5 - fileNumberString.length(); i++) {
      _fileName.concat("0");
    }
    _fileName.concat(fileNumberString);
    _fileName.concat(".CSV");

    if (!SD.exists(_fileName)) {
      return _fileName;
    }
    fileNumber++;
  }
  return "";
}
