// Wrapper for a AS5600 sensor. It uses the FRSensor class, such that the Logger class can log the sensor.
// 
// 2024-03-15, Jos Meuleman & Christian Wong, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRAS5600_h
#define FRAS5600_h

#include <FRSensor.h>
#include <AS5600.h>
#include <Wire.h>

class FRAS5600 : public FRSensor {
	public:
		FRAS5600();
		~FRAS5600();
		bool Init();
		bool Init(float offsetAngle);
		
		float GetAngle() { return _myAS5600->readAngle()*AS5600_RAW_TO_DEGREES; }
		void SetOffsetAngle (float offsetAngle) {_myAS5600->setOffset(-offsetAngle); }
		void AutoOffset() {_myAS5600->setOffset(-GetAngle()); }
		
		
		String HeaderString() override;
		String SensorString() override;

	private:
		AS5600* _myAS5600;
};

#endif