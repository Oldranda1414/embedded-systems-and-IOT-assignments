#include "SmartLightingTask.h"

SmartLightingTask::SmartLightingTask(int ledPin, int pirPin, int lsPin, int lightThreshold, int setTime){
  this->lightThreshold = lightThreshold;
  led = new Led(ledPin);
  pir = new Pir(pirPin);
  ls = new LightSensor(lsPin);
  timer = new SimpleTimer(setTime);
}
  
void SmartLightingTask::init(int period, DataManager* data){
  Task::init(period, data);
  this->data = data;
  
  state = LIGHTOFF;    
}
  
void SmartLightingTask::tick(){
  updateDataValues();
  
  switch (state){
    case LIGHTOFF:
      if(isMSAllarmed()){
        this->state = DEACTIVATED;
      }
      else if((this->ls->read() < this->lightThreshold) && this->pir->isDetected()){
        this->timer->resetTimer();
        this->led->switchOn();
        this->state = LIGHTON;          
      } 
      break;
    case LIGHTON:
      if(isMSAllarmed()){
        this->led->switchOff();
        this->state = DEACTIVATED;
      }
      else if(this->pir->isDetected()){
        this->timer->resetTimer();
      }
      else if(this->timer->checkTimer() || this->ls->read() > this->lightThreshold){
        this->led->switchOff();
        this->state = LIGHTOFF;
      }
      break;
    case DEACTIVATED:
      if(!(isMSAllarmed())){
        this->state = LIGHTOFF;          
      }
      break;
  }
}

void SmartLightingTask::updateDataValues(){
  this->data->setSLSState(this->state);
}

bool SmartLightingTask::isMSAllarmed(){
  if(this->data->getMSState() == 2 || this->data->getMSState() == 3){
    return true;
  }
  else{
    return false;
  }
}