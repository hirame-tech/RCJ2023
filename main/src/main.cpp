#include <Arduino.h>

#define pin_motorA 0 //undefine
#define pin_motorB 0 //undefine
#define pin_motorC 0 //undefine
#define pin_motorD 0 //undefine

//prototype declaration
void pin_setup();
void motor(int direction,int speed);
void get_IR();

void setup() {
  pin_setup();
  Serial.begin(115200);
}

void loop() {
  
}

void pin_setup(){
  pinMode(pin_motorA,OUTPUT);
  pinMode(pin_motorB,OUTPUT);
  pinMode(pin_motorC,OUTPUT);
  pinMode(pin_motorD,OUTPUT);
}

void motor(int direction,int speed){

}