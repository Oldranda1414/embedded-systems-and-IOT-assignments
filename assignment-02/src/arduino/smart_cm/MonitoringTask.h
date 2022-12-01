#ifndef __MONITORINGTASK__
#define __MONITORINGTASK__

#include "Task.h"
#include "Sonar.h"
#include "Potentiometer.h"
#include "ServoTimer2.h"
#include "Button.h"
#include "AnalogLed.h"
#include "Led.h"

class MonitoringTask: public Task {
  float waterThreshold1;
  float waterThreshold2;
  int normalPeriod;
  int preAlarmPeriod;
  int alarmPeriod;
  int alarmLedValue;
  int servoValue;
  SonarInterface* sonar;
  PotentiometerInterface* pot;
  ServoTimer2* servo;
  ButtonInterface* button;
  Light* ledOpen;
  AnalogLight* ledAlarm;
  DataManager* data;
  enum { NORMAL, PREALARM, ALARM, USERMODE } state;

public:

  /*
  ledOpenPin, ledAllarmPin, sonarTrigPin, sonarEchoPin, servoPin, buttonPin and potentiometerPin are the pins were the various devices are connected to arduino.
  temp is the current temperature.
  waterThreshold1 and waterThreshold2 are the water levels at which the monitorning system switches state.
  preAlarmPeriod and alarmPeriod are the periods for the PREALARM and ALARM states of the monitorign system. The period for the normal state is infered from the period passed in the init() method 
  */
  MonitoringTask(int ledOpenPin, int ledAlarmPin, int sonarTrigPin, int sonarEchoPin, int servoPin, int buttonPin, int potentiometerPin, int temp, float waterThreshold1, float waterThreshold2, int preAlarmPeriod, int alarmPeriod);  
  void init(int period, DataManager* data);  
  void tick();

private:
  void updateDataValues();
  void autoOpenValve();

  float floatMap(float x, float in_min, float in_max, float out_min, float out_max);
  long map(long x, long in_min, long in_max, long out_min, long out_max);

};

#endif