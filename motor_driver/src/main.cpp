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

AS5048A angleSensor(CSPIN, false);

int direction;

//極対数　14

//prototype declaration

void fets(int H_pin,int L_pin,int duty,int mode);
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

float angle_e = 30;

void loop() {
  //int val = caluculate_electorical_angle(OFFSET);
  static int val = 0;
  static unsigned long time = micros();
  if((micros() - time) > (DELAYTIME*1000 + 100)){
    Serial.print(caluculate_electorical_angle(OFFSET));
    Serial.print(",");
    Serial.print(val);
    val += 60;
    time = micros();
  }
  if(val >300) val -= 360;
  //
  switch (val){
    case 0:
      fets(HA,LA,MOTOR_SPEED,1);//0//180
      fets(HB,LB,MOTOR_SPEED,-1);
      fets(HC,LC,MOTOR_SPEED,0);
      
      break;
    case 60:
      fets(HA,LA,MOTOR_SPEED,1);//60//00
      fets(HB,LB,MOTOR_SPEED,0);
      fets(HC,LC,MOTOR_SPEED,-1);
      
      break;
    case 120:
      fets(HA,LA,MOTOR_SPEED,0);//120//60
      fets(HB,LB,MOTOR_SPEED,1);
      fets(HC,LC,MOTOR_SPEED,-1);
      
      break;
    case 180:
      fets(HA,LA,MOTOR_SPEED,-1);//180
      fets(HB,LB,MOTOR_SPEED,1);
      fets(HC,LC,MOTOR_SPEED,0);
      break;
    case 240:
      fets(HA,LA,MOTOR_SPEED,-1);//240
      fets(HB,LB,MOTOR_SPEED,0);
      fets(HC,LC,MOTOR_SPEED,1);
      break;
    case 300:
      fets(HA,LA,MOTOR_SPEED,0);//300
      fets(HB,LB,MOTOR_SPEED,-1);
      fets(HC,LC,MOTOR_SPEED,1);
      break;
    default:
      break;
  }
  Serial.println();
  /*
  fets(HA,LA,MOTOR_SPEED,1);
  fets(HB,LB,MOTOR_SPEED,0);
  fets(HC,LC,MOTOR_SPEED,-1);
  */
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

void fets(int H_pin,int L_pin,int duty,int mode){
  switch (mode){
  case 1://connect High
    analogWrite(H_pin,duty);
    analogWrite(L_pin,duty);
    break;
  case 0://free
    analogWrite(H_pin,0);
    analogWrite(L_pin,200);
    break;
  case -1://connect low
    analogWrite(H_pin,0);
    analogWrite(L_pin,0);
    break;
  default:
    Serial.printf("fets : exception of (int)mode\n");
    break;
  }
}