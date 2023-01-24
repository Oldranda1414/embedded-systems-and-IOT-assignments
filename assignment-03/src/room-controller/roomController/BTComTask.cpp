#include "BTComTask.h"
#include <Arduino.h>
#include "Scheduler.h"


BTComTask::BTComTask(int rxPin, int txPin, SmartRoom* smartRoom) : 
  rxPin(rxPin), txPin(txPin), smartRoom(smartRoom) {}
  
void BTComTask::init(int period){
  Task::init(period);
  channel = new SoftwareSerial(rxPin, txPin);
  this->disconectionCounter = 0;
  this->disconectionCounterMax = 4;
  channel->begin(9600);
}
  
void BTComTask::tick(){
  if(channel->available()){
    //setting the scheduler variable to true so that serial input is ignored
    Scheduler::setBTReceiving(true);
    this->disconectionCounter = this->disconectionCounterMax;
    char letter = (char)channel->read();
    String msg = "";
    //writing the message into msg
    while(letter != '\n'){
      msg.concat(letter);
      letter = (char)channel->read();
    }
    //extracting the two parts of the message
    String msg1 = msg.substring(0, msg.indexOf(String(DELIMITER)));
    String msg2 = msg.substring(msg.indexOf(String(DELIMITER)) + 1, msg.length());
    //modifying status of room object depending on orders received from BT
    if(msg1 == "on"){
      smartRoom->setLedState(true);
    }
    else if(msg1 == "off"){
      smartRoom->setLedState(false);
    }
    //checking if msg2 contains a number. if not it is ignored
    bool isNumber = true;
    for(int i = 0; i < msg2.length(); i++){
      if(!isDigit(msg2.charAt(i))){
        isNumber = false;
      }
    }
    if(isNumber){
      smartRoom->setServoMotorState(msg2.toInt());
    }
    
  }
  else{
    //setting scheduler variable to false only after no messages are received for disconectionCounterMax times
    if(this->disconectionCounter == 0){
      Scheduler::setBTReceiving(false);
    }
    else{
      this->disconectionCounter--;
    }
    
  }
}