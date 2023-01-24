#ifndef __CONST__
#define __CONST__

/*
 *  BT module connection:  
 *  - RX is pin 2 => to be connected to TXD of BT module
 *  - TX is pin 3 => to be connected to RXD of BT module
 *
 */ 
#define RX_BT_PIN 2
#define TX_BT_PIN 3
#define SERVOMOTOR_PIN 5
#define LED_PIN 4


 //defining the periods for the various tasks and for the scheduler
#define SCHEDULER_PERIOD 100
#define BT_COM_PERIOD 200
#define SERIAL_COM_PERIOD 200

#define MAXNTASK 20

#define TOLERANCE 10

#define DELIMITER '/'

#endif