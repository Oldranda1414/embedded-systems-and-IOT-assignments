#ifndef __SIMPLETIMER__
#define __SIMPLETIMER__

class SimpleTimer{

public:

  //set time is the time that must pass for the timer to run out (in milliseconds)
  SimpleTimer(int setTime);

  //returns true if timer has finished, false otherwise
  bool checkTimer();
  void resetTimer();    
private:
  int startTime;
  int setTime;  
};

#endif