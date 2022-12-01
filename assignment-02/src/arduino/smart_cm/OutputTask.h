#ifndef __OUTPUTTASK__
#define __OUTPUTTASK__

#include "Task.h"
#include <LiquidCrystal_I2C.h>

/* Wiring: SDA => A4, SCL => A5 */
class OutputTask: public Task {

DataManager* data;
LiquidCrystal_I2C* lcd;
public:
  OutputTask(int address, int columns, int rows);  
  void init(int period, DataManager* data);  
  void tick();

private:
  void lcdUpdate();
  void sendStatusMessage();
};

#endif