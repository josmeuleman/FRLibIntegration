# FRLibIntegration
Integration FlightRecorder Library for Arduino IDE. This library contains sublibraries and examples for making a flight recorder for an ESP32.
The library is written for the Project2.2 for Aeronautical & Precision Engineerring at Inholland, Delft.
The library is updated for Flight Recorder Board V2

Dependencies:
- FRLibBasics including: 
  - FRGeneric.h
  - FRButton.h
  - FRLED.h
  - FRRGBLED.h
  - FRTimer.h
- AS5600.h by Rob Tillaart for angular sensor (angle of attack)
- Adafruit_BMP280.h for pressure sensor (altitude)
- ESP32Servo.h for servo control
- MPU9250.h by Bolder Flight Systems for the IMU. Dependencies:
  - Eigen.h by Bolder Flight Systems
  - Units.h by Bolder Flight Systems
- Mahony by Arduino for Attitude Heading and Reference calculation on IMU signals
- ms4525do.h by Bolder Flight Systems for the pitot sensor
- TinyGPSPlus-ESP32.h by Mikal Hart for the GPS reading and processing
- SSD1306Ascii.h for OLED
- SSD1306AsciiWire.h for OLED

## FRLogger
The Logger class creates a handler for logging data to an SD cards. The FRLogger uses a generic sensor class, FRFRSensor. For the usage of FRSensor, see next section.
Methods:

	Logger();
	bool CheckSD();
	void AddSensor(FRSensor* Sensor);
	String GetLoggerFileName();
	bool IsLogging();
	bool StartLogger();
	bool StopLogger();
	String UpdateSensors();
	void WriteLogger();	

Examples:
- FRLoggerDemo.ino

## FRSensor
The FRSensor class is a parent class for the classes listed below. The Logger monitors objects of the class FRSensor. In the child-classes of FRSensor, the following methods are implemented

	String HeaderString() ; // for the top row of the log file
	String SensorString() ; // for the data row of the log file

## FRAnalog
The FRAnalog class is a class for specifically logging analog inputs such as potmeter sensor. It is derived from the FRSensor class.
Methods:

	FRAnalog();
	FRAnalog(byte pinNumber);
	FRAnalog(byte pinNumber, String headerString);
	
	void SetPinNumber(byte pinNumber);
	void SetHeaderString(String headerString);
	void SetOutputRange(float minValue, float maxValue);
	float GetValue() ;

Usage:

	#include <FRAnalog>
	...
	FRAnalog myAnalog1(PINAD, "AlphaVane[deg]"); // for the headerSting
	...
	myLogger.AddSensor(&myAnalog1);

If SetOutputRange is not used, the logged value in SensorString is 12 bit (0-4095), else it will have a value within the predefined range
If the header string is not set, it will be empty

## FRAS5600
The FRAS5600 class is a class for logging the AS5600 hall sensor, used in the alpha vane. Internally it uses the libary AS5600.h
	
	FRAS5600();
	bool Init();
	bool Init(float offsetAngle);
	
	float GetAngle();
	void SetOffsetAngle;
	void AutoOffset();

Usage:

	#include <FRAS5600>
	FRAS5600 myAlphaVane;
	...
	FRAS5600.Init();
	FRAS5600.AutoOffset();

AutoOffset will set the current value to zero.
The value logged in SensorString is angle [deg]	


## FRBMP280
The FRBMP280 class is a class for logging the BMP280 pressure sensor. Internally it uses the library Adafruit_BMP280.h

	FRBMP280();
	~FRBMP280();
	bool Init(TwoWire &myWire);
	long GetPressure() //Pascal
	float GetAltitude() //meter
	void AutoOffset() 
	void SetOffsetPressurehPa(float inPressure)

Note that the method Init expects the I2C communication to be running, using Wire.	
The values logged in SensorString are pressure [pa], altitude [m], chip temperature [deg C]	

## FRMPU9250
The FRMPU9250 class is a class for specifically logging the MPU9250 IMUsensor. It is derived from the FRSensor class.  
The MPU9250 is a 10 DoF sensor: 3 axis accelerometer, 3 axis gyro, 3 axis magnetometer, temperature. 
Internally it uses the library MPU9250.h by Bolder Flight Systems. For that you need to install the libraries Eigen.h and Units.h by Bolder Flight Systems as well.
It calculates the roll, pitch and yaw (heading) from the IMU, using MahonyAHRS.h. For that you need to install the Mahony library by Arduino
Methods:

	FRMPU9250();
	bool Init(TwoWire &myWire);

	void SetOffsetAcc(float ax0, float ay0, float az0);
	void AutoOffsetGyro();

	float GetAx();
	float GetAy();
	float GetAz();
	float GetGx();
	float GetGy();
	float GetGz();
	float GetMx();
	float GetMy();
	float GetMz();
	float GetRoll();
	float GetPitch();
	float GetHeading();
	

Usage:

	#include <FRMPU9250.h> 
	...
	FRMPU9250 myIMUSensor;
	...
	myIMUSensor.Init(Wire);
    myIMUSensor.AutoOffsetGyro();
    myIMUSensor.SetOffsetAcc(0.0, 0.0, -9.81);
	
	myLogger.AddSensor(&myIMUSensor);

Note that the method Init expects the I2C communication to be running, using Wire.
The values logged in SensorString are ax, ay, ax [m/s2], gx, gy, gz [rad/s], mx, my, mz [microT] chip temperature [deg C]	

## FRMS4525DO
The FRMS4525DO class is a class for specifically logging a Ms4525do sensor (differential pressure). This is used in the pitot sensor. It is derived from the FRSensor class. Internally it uses the library ms4525do.h 
Methods:

	FRMS4525DO();
	bool Init(TwoWire &myWire);

	float GetPressure()
	void AutoOffset()
	float GetSpeed()
	
Usage:
	
	#include <FRMS4525DO.h>
	FRMS4525DO myPitot; 
	...
	myPitot.Init();
	myPitot.AutoOffset();
	myLogger.AddSensor(&myPitot);
	
Note that the method Init expects the I2C communication to be running, using Wire. 
The values logged in SensorString are deltaPressure [Pa], velocity [m/s], chip temperature [deg C]	
	

## FRTinyGPS
The FRTinyGPS class is a class for specifically logging an TinyGPSPlus sensor. It is derived from the FRSensor class. Internally it uses the library TinyGPSPlus-ESP32.h
Methods:

	FRTinyGPS();
	bool Init();
	bool Init(float lat0Deg, float lon0Deg);

	void SetLat0( float lat0Deg );
	void SetLon0( float lon0Deg );

	float GetLatitude();
	float GetLongitude();
	float GetAltitude();
	int GetSatellites();
	float GetRelativeX();
	float GetRelativeY();
  
	bool HasValidData();
	
Usage:
	
	#include <FRTinyGPS.h>
	FRTinyGPS myGPSSensor; 
	...
	myGPSSensor.Init(LAT0, LON0);
	myLogger.AddSensor(&myGPSSensor);
	
The class FRTinyGPS calculates the relative x and y position from a given origin coordinate (lat0Deg, lon0Deg). If not set, the default coordinate is used of the model flying club EMCR
The values logged in SensorString are number of satellites found, date, time, latitude [deg], longitude [deg], relative x [m], relative y [m], altitude [m] 	


## FRPPMReceiver
The FRPPMReceiver class is a class specifically for logging ppm signals, based on the FRSensor class. It is an advanced version of the FRPPMReceiverSensor in FRLibBasics. 
The extra methods allow for adding the channels to the logfile, and states of switches can be accessed easily
Methods:

	FRPPMReceiver(byte pinNumber, byte numberOfChannels);

	void Init();
	void SetPrefix(String prefix);

	int ReadChannel(byte ChannelNumber);
	bool IsChannelHigh(byte ChannelNumber);
	triStateSwitch GetChannelTriState(byte ChannelNumber);

Usage:

	#include <FRPPMReceiverManager.h>
	FRPPMReceiver myReceiver(PINPPM, NUMBEROFCHANNELS);
	...
	myReceiver.Init();
	myLogger.AddSensor(&myReceiver);
	...
	myReceiver.ReadChannel(i);
	...
	binarySwitchState = myReceiver.IsChannelHigh(i);
	...
	tripleSwitchState = myReceiver.GetChannelTriState(i);
	
If the FRPPMReceiver object is added to the logger, the channels will be logged. With SetPrefix, the header prefix can be set: SetPrefix("switch") --> "switch1", "switch2" etc

## Other examples

**FRLoggerServoIntegrationTemplate**

Template for integrating two jobs that run at different frequency. In the template the logger functions get called at 10 Hz. The servo (and other) functions are colled in the main loop, running at 100 Hz. 