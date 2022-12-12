#include <Arduino.h>
#include <AS5048A.h>
#define CSPIN D6
#define HA D0
#define HB D1
#define HC D2
#define LA D3
#define LB D4
#define LC D5

#define OFFSET 180 + 10 //機械角で
#define MOTOR_SPEED 90

#define DELAYTIME 4

#define OFFSET 0//30
int old;

const int pins[] = {HA,LA,HB,LB,HC,LC};

AS5048A angleSensor(CSPIN,false);

//prototype declaration

int caluculate_electorical_angle(int offset);

void setup() {
  Serial.begin(9600);
  angleSensor.begin();
  pinMode(D7,INPUT);
  analogWriteFreq(20000);//5kHz
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

void loop() {
  static float old_val = 0;
  //int val = caluculate_electorical_angle(OFFSET);
  float val = angleSensor.getRotationInDegrees();
  String error = angleSensor.getErrors();
  //delay(10);
  if(error == ""){
    Serial.println(val);
    old_val = val;
  }else{
    Serial.print(error);
    Serial.print("  ");
    Serial.println(old_val);
  }
  delay(10);
}

int caluculate_electorical_angle(int offset){
  static float old_val;
  float tmp = angleSensor.getRotationInDegrees();

  if(tmp != 180.0000){
    float val = tmp;
    //caluculate electorical angle
    float angle_electorical = val * 14  + offset;
    angle_electorical = angle_electorical - 360*((int)angle_electorical / 360);
    if(angle_electorical < 0){
      angle_electorical += 360;
    }else if(angle_electorical > 360){
      angle_electorical -= 360;
    }
    angle_electorical = ((int)angle_electorical / 60) *60;
    old_val = angle_electorical;
    return angle_electorical;
  }else{
    return old_val;
  }

}