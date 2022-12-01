#ifndef __PIR__
#define __PIR__

#include "PirInterface.h"

class Pir : public PirInterface{

public:
  Pir(int pin);
  bool isDetected();    
private:
  int pin;  
};

#endif