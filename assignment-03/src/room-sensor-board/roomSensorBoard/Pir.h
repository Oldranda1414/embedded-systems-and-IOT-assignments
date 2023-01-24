#ifndef __PIR__
#define __PIR__

#include "Arduino.h"
#include "Component.h"

class Pir : public Component {

  public:
    Pir(const int pin); 
    bool isDetected();
};

#endif