// Class for receiving PPM signals, and add it to logger
// 
// 2023-04-16, Jos Meuleman, Inholland Aeronautical & Precision Engineering, The Netherlands

#include "Arduino.h"
#include "FRPPMReceiver.h"
#include "FRGeneric.h"

FRPPMReceiver* FRPPMReceiver::_instance = nullptr;

/*volatile unsigned long FRPPMReceiver::_lastPulseUS;
volatile uint16_t FRPPMReceiver::_channel;
volatile int* FRPPMReceiver::_channelValues;
*/
FRPPMReceiver::FRPPMReceiver(byte pinNumber, byte numberOfChannels) {
  _instance = this; // Store a pointer to this instance
  _numberOfChannels = numberOfChannels;
  _pinNumber = pinNumber;
 
  if (_channelValues != 0) {
    delete[] _channelValues;
  }
  _channelValues = new int[_numberOfChannels];
  
  for (byte i = 0; i < _numberOfChannels; i++) {
	_channelValues[i] = 0;
  }
  _channel = 0;
}

FRPPMReceiver::~FRPPMReceiver() {
  detachInterrupt(_pinNumber);
  delete[] _channelValues;
}

void FRPPMReceiver::Init(){
  pinMode(_pinNumber, INPUT);
  attachInterruptArg(digitalPinToInterrupt(_pinNumber), ppm_isr, this, CHANGE);
  _lastPulseUS = micros();
}


void FRPPMReceiver::SetPrefix(String prefix) {
  _prefix = prefix; 
}

int FRPPMReceiver::ReadChannel(byte ChannelNumber) {
  if ((ChannelNumber >= 0) && (ChannelNumber < _numberOfChannels)) {
    return (_channelValues[ChannelNumber]);
  } 
  else {
    return (0);
  }
}

bool FRPPMReceiver::IsChannelHigh(byte ChannelNumber){
	return (_channelValues[ChannelNumber] < PPMTHRESHMID);
}

triStateSwitch FRPPMReceiver::GetChannelTriState(byte ChannelNumber){
  if (_channelValues[ChannelNumber] < PPMTHRESHLOW) {
    return HISTATE;
  } else if (_channelValues[ChannelNumber] < PPMTHRESHHIGH) {
    return MIDSTATE;
  } else {
    return LOSTATE;
  }
}


String FRPPMReceiver::HeaderString(){
  String tempString;
  for (byte i = 0; i < _numberOfChannels; i++) {
	tempString.concat(_prefix);
	tempString.concat(i);
	tempString.concat("; ");
  }
  return tempString;
}

String FRPPMReceiver::SensorString(){
  String tempString;
  for (byte i = 0; i < _numberOfChannels; i++) {
	tempString.concat(createFloatString(_channelValues[i], 2));  
  }
  return tempString;
}

void FRPPMReceiver::CountPulse() {
  unsigned long tNow = micros();
  if (digitalRead(_pinNumber)) {
    _lastPulseUS = micros();
  } else {
    long x = tNow - _lastPulseUS;
    _lastPulseUS = tNow;
    if (x > _PULSETHRESHUS) {
      _channel = 0;
    }
    else {
	  if (_channel < _numberOfChannels) {
	    _channelValues[_channel] = x;
        _channel++;
	  }
	}
  }  
}

void IRAM_ATTR FRPPMReceiver::ppm_isr(void* arg) {
  FRPPMReceiver* receiver = reinterpret_cast<FRPPMReceiver*>(arg);
  receiver->CountPulse();
}
