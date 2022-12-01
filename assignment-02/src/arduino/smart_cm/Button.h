#ifndef __BUTTON__
#define __BUTTON__

#include "ButtonInterface.h"

class Button : public ButtonInterface{

public:

  //temp is the temperature in celsius
  Button(int pin);

  //TODO CHECK RANGE OF OUTPUT
  bool isPressed();

private:
  int pin;
};

#endif