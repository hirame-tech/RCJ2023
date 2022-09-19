#include <Arduino.h>
#include <AS5048A.h>
#define CSPIN D6
#define HA D0
#define HB D1
#define HC D2
#define LA D3
#define LB D4
#define LC D5

#define OFFSET 5//機械角
#define MOTOR_SPEED 127
int offset = 30;
int old;

const int pins[] = {HA,LA,HB,LB,HC,LC};

AS5048A angleSensor(CSPIN, false);

int direction;

//極対数　14

//prototype declaration

void fets(int H_pin,int L_pin,int duty,int mode);

void setup() {
  Serial.begin(9600);
  angleSensor.begin();
  pinMode(D7,INPUT);
  for (int i = 0; i < 6; i++){
    pinMode(pins[i],OUTPUT);
    digitalWrite(pins[i],LOW);
  }
  //LEDs
	pinMode(17,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(25,OUTPUT);
  digitalWrite(17,HIGH);//OFF
  digitalWrite(16,HIGH);//OFF
  digitalWrite(25,HIGH);//OFF
  delay(2000);
}
float angle_e = 30;
void loop() {
  float val;
  
  val = angleSensor.getRotationInDegrees();//OFFSET;
  if(val < 0){
    val += 360;
  }else if(val > 360){
    val -= 360;
  }
  //caluculate electorical angle
  //angle_e = val * 14  + offset;
  //angle_e = angle_e - 360*((int)angle_e / 360);
  /*
  if((millis() - old) > 100){
      angle_e += 60;
      old = millis();
  }
  */
 /*
  if(angle_e < 0){
    angle_e += 360;
  }else if(angle_e > 360){
    angle_e -= 360;
  }
  
  //u
  if(angle_e < 120){
    digitalWrite(16,LOW);
    fets(HA,LA,MOTOR_SPEED,1);//high
  }else if((180 < angle_e) && (angle_e < 300)){
    fets(HA,LA,MOTOR_SPEED,-1);//low
    digitalWrite(16,LOW);
  }else{
    fets(HA,LA,MOTOR_SPEED,0);//free
    digitalWrite(16,HIGH);
  }

  //v
  if((300 < angle_e) || (angle_e < 60)){
    fets(HB,LB,MOTOR_SPEED,-1);//low
    digitalWrite(17,LOW);
  }else if((120 < angle_e) && (angle_e < 240)){
    fets(HB,LB,MOTOR_SPEED,1);//high
    digitalWrite(17,LOW);
  }else{
    fets(HB,LB,MOTOR_SPEED,0);
    digitalWrite(17,HIGH);
  }

  //w
  if((60 < angle_e) && (angle_e < 180)){
    fets(HC,LC,MOTOR_SPEED,-1);//low
    digitalWrite(25,LOW);
  }else if(240 < angle_e){  
    fets(HC,LC,MOTOR_SPEED,1);//high
    digitalWrite(25,LOW);
  }else{
    fets(HC,LC,MOTOR_SPEED,0);//free
    digitalWrite(25,HIGH);
  }

  Serial.printf("encorder: %6.2f,offset: %4d\n",angle_e,offset);
  if((millis() - old) > 3000){
      offset += 1;
      old =millis();
  }
  */
  fets(HA,LA,MOTOR_SPEED,1);
  fets(HB,LB,MOTOR_SPEED,1);
  fets(HC,LC,MOTOR_SPEED,1);
}

void fets(int H_pin,int L_pin,int duty,int mode){
  switch (mode){
  case 1://connect High
    analogWrite(H_pin,duty);
    analogWrite(L_pin,255);
    break;
  case 0://free
    analogWrite(H_pin,0);
    analogWrite(L_pin,200);
    break;
  case -1://connect low
    analogWrite(H_pin,0);
    analogWrite(L_pin,duty);
    break;
  default:
    Serial.printf("fets : exception of (int)mode\n");
    break;
  }
}