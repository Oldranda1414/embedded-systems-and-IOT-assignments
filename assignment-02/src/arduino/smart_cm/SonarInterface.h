#ifndef __SONARINTERFACE__
#define __SONARINTERFACE__

class SonarInterface {
public:
  //returns the distance in meters
  virtual float read() = 0;
};

#endif