#ifndef __LIGHTSENSOR__
#define __LIGHTSENSOR__

#include "LightSensorInterface.h"

class LightSensor : public LightSensorInterface{

public:
  LightSensor(int pin);
  int read();    
private:
  int pin;  
};

#endif