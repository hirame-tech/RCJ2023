#include <Arduino.h>
#include <AS5048A.h>
#define CSPIN D6
#define HA D0
#define LA D3
#define HB D1
#define LB D4
#define HC D2
#define LC D5

#define OFFSET 0

const uint8_t pins[] = {HA,LA,HB,LB,HC,LC};

AS5048A angleSensor(CSPIN, false);

int direction;


void setup() {
  Serial.begin(9600);
  angleSensor.begin();
  for (int i = 0; i < 6; i++){
    pinMode(pins[i],OUTPUT);
    digitalWrite(pins[i],LOW);
  }
	pinMode(17,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(25,OUTPUT);
  digitalWrite(17,HIGH);
  digitalWrite(16,HIGH);
  digitalWrite(25,HIGH);
}

void loop() {
  float val = angleSensor.getRotationInDegrees();
  val = val + OFFSET; 
  if(val < 0){
    val += 360;
  }else if(val > 360){
    val -= 360;
  }/*
  if((0 < val) && (val < 120)){
    digitalWrite(16,HIGH);
  }else if((180 < val) && (val < 300)){
    digitalWrite(16,HIGH);
  }else{
    digitalWrite(16,HIGH);
  }
  */
  if(val < 180){
    digitalWrite(16,HIGH);
    analogWrite(HB,200);
    analogWrite(LB,0);
    analogWrite(HA,0);
    analogWrite(LA,200);
  }else{
    analogWrite(HB,0);
    analogWrite(LB,200);
    analogWrite(HA,200);
    analogWrite(LA,0);
    digitalWrite(16,LOW);
  }
  Serial.println(val);

}