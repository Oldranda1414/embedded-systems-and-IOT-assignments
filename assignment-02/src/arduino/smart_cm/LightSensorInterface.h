#ifndef __LIGHTSENSORINTERFACE__
#define __LIGHTSENSORINTERFACE__

class LightSensorInterface {
public:
  //read returns a value in range 0-1023 where 1023 is max brightness
  virtual int read() = 0;
};

#endif