#include "SimpleTimer.h"
#include "Arduino.h"

SimpleTimer::SimpleTimer(int setTime){
  this->setTime = setTime;
  this->startTime = millis();
}

bool SimpleTimer::checkTimer(){
  if((this->startTime + this->setTime) <= millis()){
    return true;
  }
  else{
    return false;
  }
}

void SimpleTimer::resetTimer(){
  this->startTime = millis();
}