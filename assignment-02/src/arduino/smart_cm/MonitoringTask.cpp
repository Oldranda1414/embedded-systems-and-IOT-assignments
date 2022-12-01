#include "MonitoringTask.h"
#include <Arduino.h>

MonitoringTask::MonitoringTask(int ledOpenPin, int ledAlarmPin, int sonarTrigPin, int sonarEchoPin, int servoPin, int buttonPin, int potentiometerPin, int temp, float waterThreshold1, float waterThreshold2, int preAlarmPeriod, int alarmPeriod){
  this->waterThreshold1 = waterThreshold1;
  this->waterThreshold2 = waterThreshold2;
  this->preAlarmPeriod = preAlarmPeriod;
  this->alarmPeriod = alarmPeriod;
  this->ledOpen = new Led(ledOpenPin);
  this->ledAlarm = new AnalogLed(ledAlarmPin);
  this->sonar = new Sonar(sonarTrigPin, sonarEchoPin, temp);
  this->servo = new ServoTimer2();
  this->servo->attach(servoPin);
  this->button = new Button(buttonPin);
  this->pot = new Potentiometer(potentiometerPin);
}
  
void MonitoringTask::init(int period, DataManager* data){
  Task::init(period, data);
  this->normalPeriod = period;
  this->data = data;
  this->ledOpen->switchOn();
  this->alarmLedValue = 0;
  this->servoValue = 0;
  state = NORMAL;    
  Serial.begin(9600);
}
  
void MonitoringTask::tick(){
  updateDataValues();
  
  switch (state){
    case NORMAL:
      if(this->sonar->read() < this->waterThreshold1){
        Task::myPeriod = this->preAlarmPeriod;
        state = PREALARM;
      }
      break;
    case PREALARM:
      if(this->sonar->read() < this->waterThreshold2){
        this->ledOpen->switchOff();
        this->ledAlarm->switchOnMax();
        Task::myPeriod = this->alarmPeriod;
        state = ALARM;
      }
      else if(this->sonar->read() > this->waterThreshold1){
        this->ledAlarm->switchOff();
        Task::myPeriod = this->normalPeriod;
        state = NORMAL;
      }
      else{
        this->alarmLedValue = this->alarmLedValue + 51;
        if(this->alarmLedValue == 255){
          this->alarmLedValue = 0;
        }        
        this->ledAlarm->switchOn(this->alarmLedValue);
      }
      break;
    case ALARM:
      if(this->sonar->read() > this->waterThreshold2){
        this->ledOpen->switchOn();
        this->servo->write(750);
        Task::myPeriod = this->preAlarmPeriod;
        state = PREALARM;
      }
      else if(this->button->isPressed()){
        state = USERMODE;
      }
      else{
        autoOpenValve();
      }
      break;
    case USERMODE:
      this->servoValue = map(this->pot->read(), 0, 1023, 750, 2250);
      this->servo->write(this->servoValue);
      if(this->button->isPressed()){
        state = ALARM;
      }
      break;
  }
}

void MonitoringTask::updateDataValues(){
  this->data->setMSState(this->state);
  this->data->setCurrentWL(this->sonar->read());
  this->data->setCurrentValveOpening(this->servoValue);
}

void MonitoringTask::autoOpenValve(){
  this->servoValue = int(MonitoringTask::floatMap(this->sonar->read(), 0, 5, 2250, 750));
  this->servo->write(this->servoValue);
}

long MonitoringTask::map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float MonitoringTask::floatMap(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
