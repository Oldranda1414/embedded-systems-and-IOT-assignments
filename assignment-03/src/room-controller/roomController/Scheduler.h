#ifndef __SCHEDULER__
#define __SCHEDULER__

#include "Task.h"
#include "Const.h"

#define MAX_TASKS 50

class Scheduler {
  
  static bool receivingBT;
  

  int basePeriod;
  int nTasks;
  Task* taskList[MAXNTASK];

public:
  Scheduler(const unsigned long period);
  void init();
  virtual bool addTask(Task* task);
  virtual void schedule();
  static bool isBTReceiving();
  static void setBTReceiving(bool receivingBT);

};

#endif
