#include "SerialComTask.h"
#include <Arduino.h>
#include "Scheduler.h"

SerialComTask::SerialComTask(SmartRoom* smartRoom) : 
  smartRoom(smartRoom) {
    msSent = false;
  }
  
void SerialComTask::init(int period){
  Task::init(period);
  Serial.begin(9600);
}
  
void SerialComTask::tick(){
  //if a message has not yet been sent to answer the server, then a message on the current status of roomController is sent
  if(!msSent){
    msSent = true;
    String msg;
    if(smartRoom->getLedState()){
      msg = "ON";
    }
    else{
      msg = "OFF";
    }
    msg = msg + String(DELIMITER);
    msg = msg + String(smartRoom->getServoMotorState());
    Serial.println(msg);
  }
  else{
    int msg = Serial.read();
    if(msg != -1){
      msSent = false;
      String str = "";
      char letter = (char)msg;
      //writing the message into msgStr
      while(letter != '\n'){
        str.concat(letter);
        letter = (char)Serial.read();
      }
      String ledStateCommand = str.substring(0, str.indexOf(DELIMITER));
      String servoStateCommand = str.substring((str.indexOf(DELIMITER) + 1), str.length());
      if(!(Scheduler::isBTReceiving())){
        if(ledStateCommand == "ON"){
          smartRoom->setLedState(true);
        }
        else if(ledStateCommand == "OFF"){
          smartRoom->setLedState(false);
        }
        smartRoom->setServoMotorState(servoStateCommand.toInt());
      }
    }
  }
  
  
}