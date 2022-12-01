#ifndef __ANALOGLED__
#define __ANALOGLED__

#include "AnalogLight.h"

class AnalogLed : public AnalogLight{ 
public:
  AnalogLed(int pin);
  void switchOn(int value);
  void switchOnMax();
  void switchOff();  
  int getState();  
private:
  int pin;
};

#endif