#include "Scheduler.h"
#include "TimerOne.h"

volatile bool timerFlag;

void timerHandler(void) {
  timerFlag = true;
}

bool Scheduler::receivingBT;

Scheduler::Scheduler(const unsigned long basePeriod) : basePeriod(basePeriod) {}

void Scheduler::init() {
  timerFlag = false;
  Scheduler::receivingBT = false;
  long period = 1000l*basePeriod;
  Timer1.initialize(period);
  Timer1.attachInterrupt(timerHandler);
  nTasks = 0;
}

bool Scheduler::addTask(Task* task) {
  if (nTasks < MAXNTASK-1) {
    taskList[nTasks] = task;
    nTasks++;
    return true;
  } else {
    return false; 
  }
}
  
void Scheduler::schedule() {   
  while (!timerFlag) {}
  timerFlag = false;
  for (int i = 0; i < nTasks; i++) {
    if (taskList[i]->updateAndCheckTime(basePeriod)) {
      taskList[i]->tick();
    }
  }
}

bool Scheduler::isBTReceiving(){
  return Scheduler::receivingBT;
}

void Scheduler::setBTReceiving(bool state){
  Scheduler::receivingBT = state;
}