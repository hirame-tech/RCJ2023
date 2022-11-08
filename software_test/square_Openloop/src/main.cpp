#include <Arduino.h>
#include <SoftwareSerial.h>
#include <AS5048A.h>
#define CSPIN D6
#define HA D0
#define HB D1
#define HC D2
#define LA D3
#define LB D4
#define LC D5
SoftwareSerial myserial(D7,17);

#define OFFSET -215 //電気角//260
#define PI 3.1415926535

#define RPM 1000
#define VOLT 8.2
#define CAL_S(r,v) r/(400*v)*255
#define CAL_D(r)   10000000/(r*7) * 1.5

#define MOTOR_SPEED CAL_S(RPM,VOLT)
#define DELAYTIME CAL_D(RPM)

int old;

const int pins[] = {HA,LA,HB,LB,HC,LC};

AS5048A angleSensor(CSPIN,false);

int direction;
int angle = 0;
int n;
int val_sq = 0;
//極対数　14

//prototype declaration

void fets(int H_pin,int L_pin,int duty,int mode);
int caluculate_electorical_angle(int offset);
void square_wave_drive(int angle);
int sign(int number);

void setup() {
  
  analogWriteFreq(60000);//60kHz
  for (int i = 0; i < 6; i++){
    pinMode(pins[i],OUTPUT);
    //digitalWrite(pins[i],LOW);
  }
  fets(HA,LA,MOTOR_SPEED,0);
  fets(HB,LB,MOTOR_SPEED,0);
  fets(HC,LC,MOTOR_SPEED,0);

  //始動シーケンス1
  square_wave_drive(0);
  delay(100);
  fets(HA,LA,MOTOR_SPEED,0);
  fets(HB,LB,MOTOR_SPEED,0);
  fets(HC,LC,MOTOR_SPEED,0);
  delay(100);

}

void setup1(){
  Serial.begin(9600);
  myserial.begin(9600);
  angleSensor.begin();
}

void loop() {
  static unsigned long loop_time = micros();
  static unsigned long time_sq = micros();

  //矩形波駆動用
  
  if((micros() - time_sq) > (DELAYTIME)){
    val_sq += 60;
    time_sq = micros();
  }
  if(val_sq >= 360) val_sq -= 360;
  if(val_sq <  0) val_sq += 360;
  square_wave_drive(val_sq);

  n = micros() - loop_time; 
  loop_time = micros();
}

void loop1(){
  angle = caluculate_electorical_angle(OFFSET);

  Serial.print(val_sq);
  Serial.print(",");
 
  Serial.println(angle);

  if(angle >= 360) angle -= 360;
  if(angle <  0) angle += 360;

}

int caluculate_electorical_angle(int offset){
  static float old_val;
  float tmp = angleSensor.getRotationInDegrees();
  String error = angleSensor.getErrors();

  if(error == ""){
    float val = tmp;
    //caluculate electorical angle
    float angle_electorical = val * 7  - offset;
    angle_electorical = angle_electorical - 360*((int)angle_electorical / 360);

    if(angle_electorical < 0){
      angle_electorical += 360;
    }else if(angle_electorical > 360){
      angle_electorical -= 360;
    }

    int angle_electorical_int = 360 - ((int)angle_electorical / 60) * 60;
    //angle_electorical_int -= 60;
    //if(angle_electorical_int == -60){
    //  angle_electorical_int = 360;
    //}
    old_val = angle_electorical_int;
    
    return angle_electorical_int;
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
    analogWrite(L_pin,255);
    break;
  default:
    Serial.printf("fets : exception of (int)mode\n");
    break;
  }
}

void square_wave_drive(int angle){
  static int offset = 0;
  switch (angle){
    case 0:
      fets(HA,LA,MOTOR_SPEED,1);//0//60
      fets(HB,LB,0,1);
      fets(HC,LC,MOTOR_SPEED,0);
      break;
    case 60:
      fets(HA,LA,MOTOR_SPEED,1);//60//120
      fets(HB,LB,MOTOR_SPEED,0);
      fets(HC,LC,0,1);
      break;
    case 120:
      fets(HA,LA,MOTOR_SPEED,0);//120//180
      fets(HB,LB,MOTOR_SPEED,1);
      fets(HC,LC,0,1);
      break;
    case 180:
      fets(HA,LA,0,1);//180//240
      fets(HB,LB,MOTOR_SPEED,1);
      fets(HC,LC,MOTOR_SPEED,0);
      break;
    case 240:
      fets(HA,LA,0,1);//240//300
      fets(HB,LB,MOTOR_SPEED,0);
      fets(HC,LC,MOTOR_SPEED,1);
      break;
    case 300:
      fets(HA,LA,MOTOR_SPEED,0);//300//0
      fets(HB,LB,0,1);
      fets(HC,LC,MOTOR_SPEED,1);
      break;
    default:
      fets(HA,LA,MOTOR_SPEED,0);
      fets(HB,LB,MOTOR_SPEED,0);
      fets(HC,LC,MOTOR_SPEED,0);
      break;
  }
}