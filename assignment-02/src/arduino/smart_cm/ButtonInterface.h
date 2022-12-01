#ifndef __BUTTONINTERFACE__
#define __BUTTONINTERFACE__

class ButtonInterface {
public:
  virtual bool isPressed() = 0;
};

#endif