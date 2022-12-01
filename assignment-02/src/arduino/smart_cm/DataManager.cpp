#include "DataManager.h"

DataManager::DataManager() {
  this->SLSState = 0;
  this->MSState = 0;
  this->waterLevel = 0;
  this->valveOpening = 0;

}

void DataManager::setSLSState(int state){
  this->SLSState = state;
}
int DataManager::getSLSState(){
  return this->SLSState;
}

void DataManager::setMSState(int state){
  this->MSState = state;
}
int DataManager::getMSState(){
  return this->MSState;
}

void DataManager::setCurrentWL(float value){
  this->waterLevel = value;
}
float DataManager::getCurrentWL(){
  return this->waterLevel;
}

void DataManager::setCurrentValveOpening(int value){
  this->valveOpening = value;
}
int DataManager::getCurrentValveOpening(){
  return this->valveOpening;
}