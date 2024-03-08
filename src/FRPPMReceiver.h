// Class for receiving PPM signals
// It uses the FRSensor class, such that the Logger class can log the sensor.
//
// 2024-03-07, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#ifndef FRPPMReceiver_h
#define FRPPMReceiver_h

#include <FRSensor.h>

class FRPPMReceiver : public FRSensor{
  public:
  FRPPMReceiver(int pinNumber, int numberOfChannels);
  ~FRPPMReceiver();
  
  void Init();
  void SetPrefix(String prefix);
  
  int ReadChannel(int ChannelNumber);
  
  String HeaderString() override;
  String SensorString() override;
  
  private:
  int _pinNumber;
  int _numberOfChannels;
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