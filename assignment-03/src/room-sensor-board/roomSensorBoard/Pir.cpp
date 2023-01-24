#include "Pir.h"

Pir::Pir(const int pin) : Component(pin) {
  pinMode(pin, INPUT);
}



bool Pir::isDetected() {
  return digitalRead(pin);
}