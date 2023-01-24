#ifndef __PHOTORESISTOR__
#define __PHOTORESISTOR__   

#include "Arduino.h"
#include "Sensor.h"

class Photoresistor : public Sensor {
  
  public:
    Photoresistor(const int pin);

    //the return value is in range(0, 1023)
    int read();
};

#endif