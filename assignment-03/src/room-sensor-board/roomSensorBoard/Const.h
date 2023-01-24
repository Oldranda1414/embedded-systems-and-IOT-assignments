#ifndef __CONST__
#define __CONST__


//defining pins
#define LED_PIN 4
#define PIR_PIN 5
#define PHOTORES_PIN 6

//start of wifi/mttq constants
#define MSG_BUFFER_SIZE  50

/* 
wifi network info
ssid and password are taken from an android device used as Router
ssid and password may vary between devices
*/
const char* ssid = "AndroidAP";
const char* password = "hycr8780";

/* MQTT server address */
const char* mqtt_server = "broker.mqtt-dashboard.com";

/* MQTT topic */
const char* topic = "Leonardo";

//end of wifi/mttq constants

#define DELAY_PERIOD 200
#define MSG_PERIOD 200

#endif