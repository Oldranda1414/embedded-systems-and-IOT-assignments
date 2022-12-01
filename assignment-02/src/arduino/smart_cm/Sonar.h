#ifndef __SONAR__
#define __SONAR__

#include "SonarInterface.h"

class Sonar : public SonarInterface{

public:
  //temp is the temperature in celsius
  Sonar(int trigPin, int echoPin, int temp);
  float read();

private:
  float vs;
  int trigPin;
  int echoPin;  
};

#endif