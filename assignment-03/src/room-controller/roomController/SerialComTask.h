#ifndef __SERIALCOMTASK__
#define __SERIALCOMTASK__

#include "Task.h"
#include "SmartRoom.h"

class SerialComTask : public Task {
  SmartRoom* smartRoom;
  bool msSent;

public:
  SerialComTask(SmartRoom* smartRoom);  
  void init(int period);  
  void tick();
};

#endif