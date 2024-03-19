// Class for receiving PPM signals
// It uses the FRSensor class, such that the Logger class can log the sensor.
//
// 2024-03-07, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRPPMReceiver_h
#define FRPPMReceiver_h

#include <FRSensor.h>

// Threshold for discrete PPM signals
const int PPMTHRESHLOW = 850;
const int PPMTHRESHMID = 1100;
const int PPMTHRESHHIGH = 1350;

// Some switches have three states. We make constants defining LOSTATE (-1), MIDSTATE (0) and HISTATE(1)
typedef enum triStateSwitch {
  LOSTATE = -1,
  MIDSTATE,
  HISTATE
};

class FRPPMReceiver : public FRSensor{
  public:
  FRPPMReceiver(byte pinNumber, byte numberOfChannels);
  ~FRPPMReceiver();
  
  void Init();
  void SetPrefix(String prefix);
  
  int ReadChannel(byte ChannelNumber);
  bool IsChannelHigh(byte ChannelNumber);
  triStateSwitch GetChannelTriState(byte ChannelNumber);
  
  String HeaderString() override;
  String SensorString() override;
  
  private:
  byte _pinNumber;
  byte _numberOfChannels;
  static volatile unsigned long _lastPulseUS;
  static volatile uint16_t _channel;
  static volatile int* _channelValues;
  
  static FRPPMReceiver* _instance;
  const int _MAXPULSEWIDTH = 1800;
  const int _MINPULSEWIDTH = 400;
  const int _PULSETHRESHUS = 3000; // Threshold for pulse duration in microseconds
  String _prefix = "Ch";  
  
  void CountPulse();
  static void ppm_isr(void* arg);  
  

};


#endif