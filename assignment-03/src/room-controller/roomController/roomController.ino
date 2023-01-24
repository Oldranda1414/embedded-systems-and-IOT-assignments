#include "Const.h"
#include "Scheduler.h"
#include "ServoMotor.h"
#include "Led.h"
#include "SmartRoom.h"
#include "BTComTask.h"
#include "SerialComTask.h"


Scheduler sched(SCHEDULER_PERIOD);

void setup(){
  ServoMotor* servoMotor = new ServoMotor(SERVOMOTOR_PIN, TOLERANCE);
  Led* led = new Led(LED_PIN);
  SmartRoom* smartRoom = new SmartRoom(servoMotor, led);
  BTComTask* btComTask = new BTComTask(RX_BT_PIN, TX_BT_PIN, smartRoom);
  SerialComTask* serialComTask = new SerialComTask(smartRoom);
  //initializing scheduler
  sched.init();
  
  //initializing all tasks and adding them to the scheduler
  btComTask->init(BT_COM_PERIOD);
  sched.addTask(btComTask);

  serialComTask->init(SERIAL_COM_PERIOD);
  sched.addTask(serialComTask);
}

void loop(){
  sched.schedule();
}