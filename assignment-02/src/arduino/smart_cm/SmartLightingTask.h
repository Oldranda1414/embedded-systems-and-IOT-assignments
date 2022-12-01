#ifndef __SMARTLIGHTINGTASK__
#define __SMARTLIGHTINGTASK__

#include "Task.h"
#include "Led.h"
#include "Pir.h"
#include "LightSensor.h"
#include "SimpleTimer.h"

class SmartLightingTask: public Task {

  int lightThreshold;
  Light* led;
  PirInterface* pir;
  LightSensorInterface* ls;
  SimpleTimer* timer;
  DataManager* data;
  enum { LIGHTOFF, LIGHTON, DEACTIVATED } state;

public:

  /*
  ledPin, pirPin and lsPin are the pins were the various devices are connected to arduino.
  lightThreshold is the threshould over which the smart lighting sistem shoul be turned off.
  setTime is the time that should pass with no detection from pir before the light is turned off (assuming no one is on the bridge anymore)
  */
  SmartLightingTask(int ledPin, int pirPin, int lsPin, int lightThreshold, int setTime);  
  void init(int period, DataManager* data);  
  void tick();

private:
  void updateDataValues();
  bool isMSAllarmed();
};

#endif