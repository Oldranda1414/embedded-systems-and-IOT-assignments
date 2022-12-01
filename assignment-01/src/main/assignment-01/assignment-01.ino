#include <avr/sleep.h>
#include <EnableInterrupt.h>

//defining the pins that I will use with more convenient names
#define BUTTON_1 12
#define BUTTON_2 11
#define BUTTON_3 10
#define BUTTON_4 9
#define RED_LED 6
#define GREEN_LED_1 5
#define GREEN_LED_2 4
#define GREEN_LED_3 3
#define GREEN_LED_4 2

//defining the pattern dimension (which also defines the number of green leds and buttons)
#define PATTERN_DIM 4

//defining the number of penalties at which a player loses  (so the player is allowed (MAX_PENALTY - 1) penalties)
#define MAX_PENALTY 3

//defining the delay time used to avoid bouncing
#define BOUNCING_AVOIDANCE_DELAY_TIME 300

//using the states enum to keep track of every state the FSM can be in
enum States{
    READY_TO_PLAY,
    GENERATING_PATTERN,
    SHOWING_PATTERN,
    WAITING_FOR_INPUT,
    SLEEPING
};

//defining the times that will be used
static unsigned long timeToRestart = 10000;
static unsigned long readyTime = 5000;
static unsigned long timeToSleep = 10000;
static unsigned long inputTime = 20000;
static unsigned long showTime = 20000;
//defining a list for the led pin names
unsigned int leds[] = {GREEN_LED_1, GREEN_LED_2, GREEN_LED_3, GREEN_LED_4};
//defining a list for the button pin names
unsigned int buttons[] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};
//defining the free pin that will be used to generete the seed for the pattern generator
const int FREE_PIN = A5;
//defining the analog pin input for the potetiometer
const int potPin = A0;
//current is used to store the current input value of the potentiometer
int current = 0;
//state will keep track of the current state of the FSM. every loop I will check it value
States state = READY_TO_PLAY;
//pattern will store the pattern for every game
int * pattern;
//level will store the current level
unsigned long level = 1;
//difficulty will store the difficulty in the current game
int difficulty = 1;
//penalty will keep track of the number of strikes the player has received
int penalty = 0;
//playerDead keeps track of the player, meaning it is true if the player is dead, false otherwise
bool playerDead = false;
//factor is the factor by which the times are shortened after every level. It depends on the difficulty selected for each game
unsigned long factor = 1;
//finishTime is used by functions to simulate a simple timer
unsigned long finishTime = 0;
//timerInUse is used to check if finishTime must be initialized or if the "timer" is currently in use
bool timerInUse = false;

//brightness and fadeAmount are used to obtain the fading led effect on the red led
int brightness = 0;
int fadeAmount = 5;

void setup() {
  //setting the pins to the correct modes
  for(int i = 0; i < PATTERN_DIM; i++){
    pinMode(buttons[i],INPUT);
    pinMode(leds[i],OUTPUT);
  }
  pinMode(RED_LED,OUTPUT);
  pinMode(potPin,INPUT);
  
  //beginning serial communication
  Serial.begin(9600);

  Serial.println("Welcome to the Catch the Led Pattern Game. Press Key T1 to Start");
}

void loop() {
  //at the start of every loop it will check the current state of the FSM

  if(state == READY_TO_PLAY){
    //starting the timer
    startTimer(timeToSleep);

    //fading the red led     
    analogWrite(RED_LED, brightness);
    brightness = brightness + fadeAmount;
    if(brightness == 0 || brightness == 255){
      fadeAmount = -fadeAmount;
    }
    delay(30);

    //read the potentiometer and set the difficulty and factor
    //if the difficulty has changed the user is notified through serial monitor
    readPot();
    int newDifficulty = map(current, 0, 1020, 1, 4);
    if(newDifficulty != difficulty){
      difficulty = newDifficulty;
      Serial.println("New selected difficulty");
      Serial.print("You are now on difficulty setting ");
      Serial.println(difficulty);
      
      //setting the factor depending on the difficulty
      if(difficulty == 1){
        factor = 1;
      }
      else if(difficulty == 2){
        factor = 2;
      }
      else if(difficulty == 3){
        factor = 3;
      }
      else if(difficulty == 4){
        factor = 4;
      }
    }
    
    //checking if the start button (T1) has been pressed
    if(digitalRead(BUTTON_1)){
      analogWrite(RED_LED, 0);
      timerInUse = false;
      Serial.println("Go!");
      changeState(GENERATING_PATTERN);
    } 
           
    //checking if timer has run out (putting arduino into sleep mode if it has)
    if(checkTimer()){
      changeState(SLEEPING);
    }
  }

  else if(state == SLEEPING){
    sleepNow();
    delay(BOUNCING_AVOIDANCE_DELAY_TIME);
    Serial.println("Welcome to the Catch the Led Pattern Game. Press Key T1 to Start");
    changeState(READY_TO_PLAY);
  }
      
  else if(state == GENERATING_PATTERN){
    //initial delay after the game has started (T1)
    delay(readyTime);
    //generating the pattern
    pattern = generatePattern(FREE_PIN);
    //turning on the green leds to form the pattern just generated
    for(int i = 0; i < PATTERN_DIM; i++){
      if(pattern[i] == 1){
        digitalWrite(leds[i], HIGH);
      }
    }
    Serial.println("Memorise the pattern!");
    changeState(SHOWING_PATTERN);
  }

  else if(state == SHOWING_PATTERN){
    //giving the player time to memorize the pattern. Note that the delay time (T2) is dictated 
    //by the difficulty (as factor depends by it) and the current level
    //starting the timer
    startTimer(showTime/(factor*level));

    //checking if the player has pressed any button and giving a penalty accordingly
    for(int i = 0; i < 4; i++){
      if(digitalRead(buttons[i])){
        addPenalty();
      }
    }

    //checking if the timer is over
    if(checkTimer()){
      Serial.println("Input the correct pattern");
      turnGreenLedsOff();
      changeState(WAITING_FOR_INPUT); 
    }
    else{
      changeState(SHOWING_PATTERN);
    }
  }

  else if(state == WAITING_FOR_INPUT){
    //giving the player time to input the pattern. Note that the delay time (T3) is dictated 
    //by the difficulty (as factor depends by it) and the current level
    //starting the timer
    startTimer(inputTime/(factor*level));
    //updating the leds as buttons are being pressed
    for(int i = 0; i < PATTERN_DIM; i++){
      checkButtonLedPair(buttons[i], leds[i]);
    }
      
    //once the timer is finished check if the pattern inputted by the player is correct and act accordingly
    if(checkTimer()){
      if(checkPattern()){
        level++;
        Serial.println("New point!");
        Serial.print("Score: ");
        Serial.println(level);
      }
      else{
        addPenalty();
      }
      turnGreenLedsOff();
      changeState(GENERATING_PATTERN);  
    }
  }
}

//checks if the pattern inputted by the player is correct.
//returns true if it is correct, false otherwise
bool checkPattern(){
  bool correct = true;
  for(int i = 0; i < PATTERN_DIM; i++){
    if(!(digitalRead(leds[i]) == pattern[i])){
      correct = false;
    }      
  }
  return correct;
}

//adds a penalty to the penalty count and checks if the penalty is deadly, displaying game over and resetting score and penalty counter if it is
//it modifies the value of playerDead, to keep track of the state of the player
//it checks if a timer was active and "turns it off" by modyfing the value of timerInUse
void addPenalty(){
  penalty++;
  Serial.println("Penalty!");
  digitalWrite(RED_LED,HIGH);
  delay(1000);
  digitalWrite(RED_LED,LOW);
  if(penalty == MAX_PENALTY){
    Serial.print("Game Over. Final Score: ");
    Serial.println(level);
    timerInUse = false;
    penalty = 0;
    level = 1;
    turnGreenLedsOff();
    playerDead = true;
    delay(timeToRestart);
  }
}

//changes the state of the FSM accoridng to the input and the current state of the player
void changeState(States newState){
  if(playerDead){
    playerDead = false;
    Serial.println("Welcome to the Catch the Led Pattern Game. Press Key T1 to Start");  
    state = READY_TO_PLAY;
  }
  else{
    state = newState;
  }
}

//sets all of the green leds pins to LOW
void turnGreenLedsOff(){
  for(int i = 0; i < PATTERN_DIM; i++){
    digitalWrite(leds[i], LOW);
  }
}

//checks if the timer has already been initialized and if not it initializes it
void startTimer(unsigned long delayTime){
  if(!timerInUse){
    timerInUse = true;
    finishTime = (millis() + delayTime);
  }
}

//checks if the timer has run out, and if necessary updates the value of timerInUse
bool checkTimer(){
  bool check = false;
  if(millis() >= finishTime){
    check = true;
    timerInUse = false;
  }
  return check;
}

//checks if a button has been pressed and if it has it switches the state of the corresponding led
void checkButtonLedPair(int buttonPin, int ledPin){
  if(digitalRead(buttonPin) == HIGH){
    int state = digitalRead(ledPin);
    if(state == HIGH){
      state = LOW;
    }
    else{
      state = HIGH;
    }
    digitalWrite(ledPin, state);
    delay(BOUNCING_AVOIDANCE_DELAY_TIME);
  }
}

//returns a pointer to a list containing the generated pattern
int * generatePattern(int pin){
  bool validPattern = false;
  randomSeed(analogRead(pin));
  static int pattern[4];
  for(int i = 0; i < 4; i++){
    pattern[i] = random(2);
    if(pattern[i] == 1){
      validPattern = true;
    }
  }
  if(!validPattern){
    pattern[0] = 1;
  }  
  return pattern;
}

//returns the value read from the potentiometer
void readPot(){
  int newValue = analogRead(potPin);
  if(newValue <= current - 5 || newValue >= current +5){
    current = newValue;    
  }
}

//function passed to enableInterrupt()
void wakeUpNow(){

}

//puts to sleep arduino attaching the button pins to interrupts to wake it up
void sleepNow(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  digitalWrite(RED_LED, LOW);
  sleep_enable();
  for(int i = 0; i < PATTERN_DIM; i++){
    enableInterrupt(buttons[i], wakeUpNow, HIGH);
  }
  
  sleep_mode();   //here the device is put to sleep
  //HERE THE PROGRAM CONTINUES AFTER WAKING UP

  sleep_disable();
  Serial.println("Waking up!");
  for(int i = 0; i < PATTERN_DIM; i++){
    disableInterrupt(buttons[i]);
  }
}