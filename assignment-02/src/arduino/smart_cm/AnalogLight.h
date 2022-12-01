#ifndef __ANALOGLIGHT__
#define __ANALOGLIGHT__

class AnalogLight {
public:
  virtual void switchOn(int) = 0;
  virtual void switchOnMax() = 0;
  virtual void switchOff() = 0;  
  virtual int getState() = 0; 
};

#endif