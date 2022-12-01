#include "Sonar.h"
#include "Arduino.h"

Sonar::Sonar(int trigPin, int echoPin, int temp){
  this->vs = 331.45 + 0.62*temp;
  this->trigPin = trigPin;
  this->echoPin = echoPin;
  pinMode(this->trigPin,OUTPUT);
  pinMode(this->echoPin,INPUT);
}

float Sonar::read(){
  //sending impulse
  digitalWrite(this->trigPin,LOW);
  delayMicroseconds(3);
  digitalWrite(this->trigPin,HIGH);
  delayMicroseconds(5);
  digitalWrite(this->trigPin,LOW);
    
  //receiving echo
  float tUS = pulseIn(echoPin, HIGH);
  float t = tUS / 1000.0 / 1000.0 / 2;
  float d = t*vs;
  return d;
}