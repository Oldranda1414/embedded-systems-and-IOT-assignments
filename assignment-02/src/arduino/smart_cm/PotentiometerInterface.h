#ifndef __POTENTIOMETERINTERFACE__
#define __POTENTIOMETERINTERFACE__

class PotentiometerInterface {
public:
  virtual int read() = 0;
};

#endif