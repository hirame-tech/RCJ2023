#include <Arduino.h>

#define AIN1 D8
#define AIN2 D9
#define BIN1 D4
#define BIN2 D5

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);

}

void loop() {
  if(Serial1.available()){
    int tmp = Serial1.read();
    Serial.println(tmp);
    int speed = tmp&B01111111;
    
    if(speed <= 100){
      speed = map(speed,0,100,-255,255);
      if(tmp&B10000000){
        if(speed > 0){
          digitalWrite(AIN1,LOW);
          analogWrite(AIN2,speed); 
        }else if(speed == 0){
          digitalWrite(AIN1,HIGH);
          digitalWrite(AIN2,HIGH);             
        }else{
          digitalWrite(AIN2,LOW);
          analogWrite(AIN1,-speed);        
        }
      }else{
        if(speed > 0){
          digitalWrite(BIN1,LOW);
          analogWrite(BIN2,speed); 
        }else if(speed == 0){
          digitalWrite(BIN1,HIGH);
          digitalWrite(BIN2,HIGH);             
        }else{
          digitalWrite(BIN2,LOW);
          analogWrite(BIN1,-speed); 
        }
      }
    }else{
      digitalWrite(AIN1,LOW);
      digitalWrite(BIN1,LOW);
      digitalWrite(AIN2,LOW);
      digitalWrite(BIN2,LOW);       
    }
  }
}