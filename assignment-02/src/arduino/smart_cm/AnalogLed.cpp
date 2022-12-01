#include "AnalogLed.h"
#include "Arduino.h"

AnalogLed::AnalogLed(int pin){
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

void AnalogLed::switchOn(int value){
  analogWrite(pin, value);
}

void AnalogLed::switchOnMax(){
  analogWrite(pin, 255);
}

void AnalogLed::switchOff(){
  analogWrite(pin, 0);
}

int AnalogLed::getState(){
  return analogRead(this->pin);
}