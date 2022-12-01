#include "OutputTask.h"
#include <Arduino.h>
#include "MsgService.h"

OutputTask::OutputTask(int address, int columns, int rows){
  this->lcd = new LiquidCrystal_I2C(address, columns, rows);
  
}
  
void OutputTask::init(int period, DataManager* data){
  Task::init(period, data);
  this->lcd->LiquidCrystal_I2C::init();
  this->data = data;
  MsgService.init();
  //Serial.begin(9600);
}
  
void OutputTask::tick(){

  //printing data through Serial (debug purpose)
  /*  
  Serial.println("printing data from the SLS");
  Serial.print("the SLS state is: ");
  Serial.print(this->data->getSLSState());
  Serial.print("\n\n");
  Serial.println("printing data from the MS");
  Serial.print("the MS state is: ");
  Serial.print(this->data->getMSState());
  Serial.print(" the current water level is: ");
  Serial.print(this->data->getCurrentWL());
  Serial.print(" the current valve opening is: ");
  Serial.print(this->data->getCurrentValveOpening());
  Serial.print("\n\n\n\n\n");
  */
  lcdUpdate();

  sendStatusMessage();
  
}

void OutputTask::lcdUpdate(){
  if(this->data->getMSState() == 0){
    this->lcd->clear();
    this->lcd->noBacklight();
  }
  else if(this->data->getMSState() == 1){
    this->lcd->clear();
    this->lcd->backlight();
    this->lcd->setCursor(0, 0);
    this->lcd->print("Pre-alarm state");
    this->lcd->setCursor(0, 1);
    this->lcd->print("WL : ");
    this->lcd->print(this->data->getCurrentWL());
  }
  else if(this->data->getMSState() == 2){
    this->lcd->clear();
    this->lcd->backlight();
    this->lcd->setCursor(0, 0);
    this->lcd->print("Alarm state");
    this->lcd->setCursor(0, 1);
    this->lcd->print("WL:");
    this->lcd->print(this->data->getCurrentWL());
    this->lcd->print(" VO:");
    this->lcd->print(map(this->data->getCurrentValveOpening() ,750 , 2250, 0, 180));
  }
  else{
    this->lcd->clear();
    this->lcd->backlight();
    this->lcd->setCursor(0, 0);
    this->lcd->print("User Mode");
    this->lcd->setCursor(0, 1);
    this->lcd->print("WL:");
    this->lcd->print(this->data->getCurrentWL());
    this->lcd->print(" VO:");
    this->lcd->print(map(this->data->getCurrentValveOpening(),750, 2250, 0, 180));
  }
}

void OutputTask::sendStatusMessage(){
  String msState = "";
  String slsState = "";
  if(this->data->getMSState() == 0){
    msState = "Normal";    
  }
  else if(this->data->getMSState() == 1){
    msState = "Pre-alarm";
  }
  else if(this->data->getMSState() == 2){
    msState = "Alarm";
  } 
  else if(this->data->getMSState() == 3){
    msState = "User mode";
  }

  if(this->data->getSLSState() == 0){
    slsState = "Light on";
  }
  else if(this->data->getSLSState() == 1){
    slsState = "Light off";
  }
  else if(this->data->getSLSState() == 2){
    slsState = "Deactivated";
  }
  String message = String(msState + String("/") + slsState + String("/") + String(this->data->getCurrentWL()));
  MsgService.sendMsg(message);
}