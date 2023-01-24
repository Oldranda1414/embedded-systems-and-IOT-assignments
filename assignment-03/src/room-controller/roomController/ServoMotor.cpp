#include "ServoMotor.h"

ServoMotor::ServoMotor(const int pin, const int tolerance) : Component(pin), tolerance(tolerance) {
    motor.attach(pin);
}

void ServoMotor::move(int percentege) {
  //mapping percentege to angle
  int newAngle = map(percentege, 0, 100, 180, 0);
  //moving the motor only if a new angle has been requested
  if((newAngle > this->angle + tolerance) || (newAngle < this->angle - tolerance)){
    motor.write(MIN_PULSE_WIDTH + newAngle*coeff);
    this->angle = newAngle;    
  }
}

int ServoMotor::getAngle() {
  return this->angle;
}

int ServoMotor::getPercentege(){
  return map(this->angle, 0, 180, 100, 0);
}