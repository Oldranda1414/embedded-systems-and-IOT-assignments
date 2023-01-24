#ifndef __LED__
#define __LED__

#include "Arduino.h"
#include "Component.h"

class Led : public Component {
  enum {ON, OFF} state;
  
  public:
    Led(const int pin); 
    void turnOn();
    void turnOff();
    bool isOn();
};

#endif