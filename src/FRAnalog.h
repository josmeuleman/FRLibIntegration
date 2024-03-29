// Wrapper for a sensor on the analog input. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-21, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRAnalog_h
#define FRAnalog_h

#include <FRSensor.h>
#include <Arduino.h>

//creates class which inherents Sensor
class FRAnalog : public FRSensor {
	public :
		FRAnalog();
		FRAnalog(byte pinNumber);
		FRAnalog(byte pinNumber, String headerString);
		~FRAnalog();

		void SetPinNumber(byte pinNumber);
		void SetHeaderString(String headerString);
		void SetOutputRange(float minValue, float maxValue);
		float GetValue() ;

		String HeaderString() override;
		String SensorString() override;

  private : 
	  byte _pinNumber;
	  int _min, _max;
	  const int _MAXAD = 4095;
	  String _headerString;
};

#endif