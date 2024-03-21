# FRLibIntegration
Integration FlightRecorder Library for Arduino IDE. This library contains sublibraries and examples for making a flight recorder for an ESP32.
The library is written for the Project2.2 for Aeronautical & Precision Engineerring at Inholland, Delft.
The library is updated for Flight Recorder Board V2

Dependencies:
- FRLibBasics
  - FRGeneric.h
  - FRButton.h
  - FRLED.h
  - FRRGBLED.h
  - FRTimer.h
- AS5600.h for angular sensor (angle of attack)
- ESP32Servo.h 
- SSD1306Ascii.h for OLED
- SSD1306AsciiWire.h for OLED


## FRPPMReceiver
The PPMReceiver class creates a listener to a PPM signal.
Methods:

    #include <FRPPMReceiver.h>
	PPMReceiver(int pinNumber, int numberOfChannels);
    void SetLowPassFilter(float* alphaValues);
    float GetLowPassFilter(int channel);
    void Update();
    float ReadChannel(int ChannelNumber);


## FRLogger
The Logger class creates a handler for logging data to an SD cards. The FRLogger uses a generic sensor class, FRFRSensor. For the usage of FRFRSensor, see next section.
Methods:

	#include <FRLogger.h>
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

	#include <FRAnalog>
	FRAnalog();
	FRAnalog(byte pinNumber);
	FRAnalog(byte pinNumber, String headerString);
	~FRAnalog();

	void SetPinNumber(byte pinNumber);
	void SetHeaderString(String headerString);
	void SetOutputRange(float minValue, float maxValue);
	float GetValue() ;

Usage:

	...
	FRAnalog myAnalog1(PINAD, "AlphaVane[deg]"); // for the headerSting
	...
	myLogger.AddSensor(&myAnalog1);

If SetOutputRange is not used, the logged value is 12 bit (0-4095)
If the header string is not set, it will be empty

## FRAS5600
The FRAS5600 class is a class for logging the AS5600 hall sensor, used in the alpha vane
	
	#include <FRAS5600>
	FRAS5600();
	~FRAS5600();
	bool Init();
	bool Init(float offsetAngle);
	
	float GetAngle();
	void SetOffsetAngle;
	void AutoOffset();

Usage:

	FRAS5600 myAlphaVane;
	...
	FRAS5600.Init();
	FRAS5600.AutoOffset();

AutoOffset will set the current value to zero.

## FRMPU6050Manager
The MPU6050Manager class is a class for specifically logging an MPU6050 sensor. It is derived from the FRSensor class. Internally it uses the library Adafruit_MPU6050
Methods:

	#include <FRMPU6050Manager.h> 
	MPU6050Manager();
	bool Init(TwoWire &myWire, mpu6050_accel_range_t accelRange, mpu6050_gyro_range_t gyroRange)

Usage:

	...
	MPU6050Manager myMPU;
	...
	myMPU.Init(Wire, MPU6050_RANGE_4_G, MPU6050_RANGE_500_DEG);
	myLogger.AddSensor(&myMPU);

Note that:
- The method init expects the I2C communication to be running
- The method init requires a range for the accelerometer and the gyro. The values are defined in the library Adafruit_MPU6050

Examples:
- FRLoggerDemo.ino

## FRTinyGPSManager
The TinyGPSManager class is a class for specifically logging an TinyGPSPlus sensor. It is derived from the FRSensor class. Internally it uses the library TinyGPSPlus
Methods:

	TinyGPSManager();
	bool Init();
	
Usage:
	
	#include <FRTinyGPSManager.h>
	TinyGPSManager myGPS; 
	...
	myGPS.Init();
	myLogger.AddSensor(&myGPS);

## FRMs4525doManager
The Ms4525doManager class is a class for specifically logging a Ms4525do sensor (differential pressure). This is used in the pitot sensor. It is derived from the FRSensor class. Internally it uses the library ms4525do
Methods:

	Ms4525doManager();
	Init();
	
	
Usage:
	
	#include <FRTinyGPSManager.h>
	Ms4525doManager myPitot; 
	...
	myPitot.Init();
	myLogger.AddSensor(&myPitot);
	
## PPMReceiverManager
The PPMReceiverManager class is a class specifically for logging ppm signals. It is a variant on PPMReceiver, but based on the FRSensor class, using an interrupt routine.
Methods:

	PPMReceiverManager(int pinNumber, int numberOfChannels)
	void Init();
	void SetPrefix(String prefix);
    int ReadChannel(int ChannelNumber);

Usage:

	#include <FRPPMReceiverManager.h>
	PPMReceiverManager MyReceiverManager(PINPPM, NUMBEROFCHANNELS);
	...
	MyReceiverManager.Init();
	myLogger.AddSensor(&MyReceiverManager);
	...
	MyReceiverManager.ReadChannel(i);

Examples:
- FRPPMReceiverManagerTest.ino	
	

## Other examples
**FRGPSTest.ino**

Read the Serial2 port and prints the data to the Serial monitor. Useful for checking connection with GPS Sensor. This example does not decode the signal, it merely prints the raw data

**FRMPU6050Test.ino**

Sets up communication to the MPU6050 over I2C

**FRServoAnalogReadTest.ino**

Uses ESP32Servo library for controlling servo's with the analog input.

**FRTimeDemo.ino**

Demonstrates the uses of millis() and the duration of print statements.