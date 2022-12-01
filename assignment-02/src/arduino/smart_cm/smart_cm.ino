#include "Scheduler.h"
#include "SmartLightingTask.h"
#include "OutputTask.h"
#include "MonitoringTask.h"

//defining the pins
#define PIR_PIN 13
#define LED_A_PIN 12
#define LED_B_PIN 4
#define LED_C_PIN 2
#define BUTTON_PIN 3
#define SONAR_ECHO_PIN 5
#define SONAR_TRIG_PIN 6

//LIGHT_THRESHOLD is the level of luminosity under which the smart lighting system turns its led on (if someone is detected on the bridge)
#define LIGHT_THRESHOLD 500
//SET_TIME is the time that passes with no pir detections before the light on the bridge must turn off
#define SET_TIME 5000
//TEMP is the current temperature in celsius. It is immagined that the temperature is 20°.
#define TEMP 20
//defining LCD data
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 20
#define LCD_ROWS 4
//defining the periods for the various tasks and for the scheduler
#define SCHEDULER_PERIOD 100
#define SLS_PERIOD 200
#define OUTPUT_PERIOD 2000
#define MS_NORMAL_PERIOD 2000
#define MS_PREALARM_PERIOD 1500
#define MS_ALARM_PERIOD 1000

//defining the light sensor pin and the potentiometer pin (both analog input pins)
const int LS_PIN = A0;
const int POT_PIN = A1;
const int SERVO_PIN = A2;

//WATER_THRESHOLD_1 and WATER_THRESHOLD_2 define the levels of water that trigger a state change in the monitoring system
const float WATER_THRESHOLD_1 = 1.0f;
const float WATER_THRESHOLD_2 = 0.5f;

DataManager* data = new DataManager();
Scheduler sched;
Task* smartLightingTask = new SmartLightingTask(LED_A_PIN, PIR_PIN, LS_PIN, LIGHT_THRESHOLD, SET_TIME);
Task* outputTask = new OutputTask(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
Task* monitoringTask = new MonitoringTask(LED_B_PIN, LED_C_PIN, SONAR_TRIG_PIN, SONAR_ECHO_PIN, SERVO_PIN, BUTTON_PIN, POT_PIN, TEMP, WATER_THRESHOLD_1, WATER_THRESHOLD_2, MS_PREALARM_PERIOD, MS_ALARM_PERIOD);

void setup(){
  //initializing scheduler
  sched.init(SCHEDULER_PERIOD);
  
  //initializing all tasks and adding them to the scheduler
  
  monitoringTask->init(MS_NORMAL_PERIOD, data);
  sched.addTask(monitoringTask);

  smartLightingTask->init(SLS_PERIOD, data);
  sched.addTask(smartLightingTask);


  outputTask->init(OUTPUT_PERIOD, data);
  sched.addTask(outputTask);

}

void loop(){
  sched.schedule();
}
