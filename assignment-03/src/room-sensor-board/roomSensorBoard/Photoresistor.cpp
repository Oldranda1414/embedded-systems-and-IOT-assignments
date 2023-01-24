#include "Photoresistor.h"

Photoresistor::Photoresistor(const int pin) : Sensor(pin) {
  pinMode(pin, INPUT);
}

int Photoresistor::read() {
  int value = analogRead(pin);
  value = map(value, 0, 1023, 0, 100);
  if(value > 100){
    value = 100;
  }
  return value;
}