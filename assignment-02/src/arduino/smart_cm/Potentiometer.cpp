#include "Potentiometer.h"
#include "Arduino.h"

Potentiometer::Potentiometer(int pin){
  this->pin = pin;
  pinMode(pin, INPUT);
}

int Potentiometer::read(){
  return analogRead(this->pin);
}