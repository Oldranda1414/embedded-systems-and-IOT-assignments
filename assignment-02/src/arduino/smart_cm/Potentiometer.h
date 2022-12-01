#ifndef __POTENTIOMETER__
#define __POTENTIOMETER__

#include "PotentiometerInterface.h"

class Potentiometer : public PotentiometerInterface{

public:

  //temp is the temperature in celsius
  Potentiometer(int pin);
  int read();

private:
  int pin;
};

#endif