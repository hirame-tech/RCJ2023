#include <Arduino.h>
#include <SoftwareSerial.h>

#define IN_A_1 D1
#define IN_B_1 D2
#define PWM1   D3
#define IN_A_2 D4
#define IN_B_2 D5
#define PWM2   D6


SoftwareSerial myserial(D7,17);

void setup() {
  Serial.begin(115200);
  myserial.begin(115200);
  pinMode(IN_A_1,OUTPUT);
  pinMode(IN_B_1,OUTPUT);
  pinMode(PWM1,OUTPUT);
  pinMode(IN_A_2,OUTPUT);
  pinMode(IN_B_2,OUTPUT);
  pinMode(PWM2,OUTPUT);

  digitalWrite(IN_A_1,LOW);
  digitalWrite(IN_B_1,LOW);
  analogWrite(PWM1,0);
  digitalWrite(IN_A_2,LOW);
  digitalWrite(IN_B_2,LOW);
  analogWrite(PWM2,0);  
}

void loop() {
  if(myserial.available()){
    int tmp = myserial.read();
    
    int speed = tmp&B01111111;
    
    if(speed <= 100){
      speed = map(speed,0,100,-255,255);
      if(tmp&B10000000){
        Serial.println(speed);
        if(speed > 0){
          digitalWrite(IN_A_1,HIGH);
          digitalWrite(IN_B_1,LOW);
          analogWrite(PWM1,speed); 
        }else if(speed == 0){
          digitalWrite(IN_A_1,HIGH);
          digitalWrite(IN_B_1,HIGH);
          analogWrite(PWM1,255);              
        }else{
          digitalWrite(IN_A_1,LOW);
          digitalWrite(IN_B_1,HIGH);
          analogWrite(PWM1,-speed);        
        }
      }else{
        if(speed > 0){
          digitalWrite(IN_A_2,LOW);
          digitalWrite(IN_B_2,HIGH);
          analogWrite(PWM2,speed);
        }else if(speed == 0){
          digitalWrite(IN_A_2,HIGH);
          digitalWrite(IN_B_2,HIGH);
          analogWrite(PWM2,255);       
        }else{
          digitalWrite(IN_A_2,HIGH);
          digitalWrite(IN_B_2,LOW);
          analogWrite(PWM2,-speed);        
        }
      }
    }else{
      digitalWrite(IN_A_1,LOW);
      digitalWrite(IN_B_1,LOW);
      analogWrite(PWM1,0);
      digitalWrite(IN_A_2,LOW);
      digitalWrite(IN_B_2,LOW);
      analogWrite(PWM2,0);        
    }
  }
}