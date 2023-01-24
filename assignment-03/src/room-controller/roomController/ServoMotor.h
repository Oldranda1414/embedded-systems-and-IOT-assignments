#ifndef __SERVOMOTOR__
#define __SERVOMOTOR__

#include "Arduino.h"
#include "Sensor.h"
#include "Const.h"
#include "ServoTimer2.h"

class ServoMotor : public Component {
  ServoTimer2 motor;
  float coeff = ((float)MAX_PULSE_WIDTH-(float)MIN_PULSE_WIDTH)/180;
  int angle = 0;
  int tolerance;

  public:
    ServoMotor(const int pin, const int tolerance); 
    void move(int percentege);
    int getAngle();
    int getPercentege();
};

#endif