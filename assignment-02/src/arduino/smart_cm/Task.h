#ifndef __TASK__
#define __TASK__

#include "DataManager.h"
class Task {
  
  int timeElapsed;
  bool active;
  DataManager* myData;
  
public:
  int myPeriod;
  
  virtual void init(int period, DataManager* data){
    myPeriod = period;  
    myData = data;
    timeElapsed = 0;
    active = true;
  }

  virtual void tick() = 0;

  bool updateAndCheckTime(int basePeriod){
    timeElapsed += basePeriod;
    if (timeElapsed >= myPeriod){
      timeElapsed = 0;
      return true;
    } else {
      return false; 
    }
  }

  bool isActive(){
    return active;
  }

  void setActive(bool active){
    this->active = active;
  }

  void changePeriod(int period){
    this->myPeriod = period;
  }
  
};

#endif
