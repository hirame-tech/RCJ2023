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

#define MAX_SPEED 50

#define OFFSET_A 
#define OFFSET_B 52
#define OFFSET_C 73
#define OFFSET_D 321

#define OFFSET  OFFSET_A//電気角
#define PI 3.1415926535

#define RPM 1000
#define VOLT 8.0
#define CAL_S(r,v) r/(400*v)*255
#define CAL_D(r)   10000000/(r*7) * 1.5

#define MOTOR_SPEED CAL_S(RPM,VOLT)
#define DELAYTIME CAL_D(RPM)

int old;

const int pins[] = {HA,LA,HB,LB,HC,LC};

AS5048A angleSensor(CSPIN,false);

int direction = 0;
int val_sq = 0;
int speed = 0;
//極対数　14

//prototype declaration

void fets(int H_pin,int L_pin,int duty,int mode);
int caluculate_electorical_angle(int offset);
void square_wave_drive(int angle,int motor_speed);

void setup() {
  analogWriteFreq(10000);//60kHz
  for (int i = 0; i < 6; i++){
    pinMode(pins[i],OUTPUT);
  }
  fets(HA,LA,MOTOR_SPEED,0);
  fets(HB,LB,MOTOR_SPEED,0);
  fets(HC,LC,MOTOR_SPEED,0);

  //始動シーケンス1
  square_wave_drive(0,MOTOR_SPEED);
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
  static unsigned long time_sq = micros();
  //矩形波駆動用
  if((micros() - time_sq) > (DELAYTIME)){
    val_sq += 60;
    time_sq = micros();
  }
  if(val_sq >= 360) val_sq -= 360;
  if(val_sq <  0) val_sq += 360;
  //square_wave_drive(val_sq,MOTOR_SPEED);
  square_wave_drive(direction,speed);
  //fets(HA,LA,100,1);
  //fets(HB,LB,100,1);
  //fets(HC,LC,MOTOR_SPEED,1);
}

void loop1(){
  int angle = 0;
  int tmp;
  if(myserial.available()){
    tmp = myserial.read();
    speed = (tmp - 127)*2;
    if(abs(speed) > MAX_SPEED){
      speed = MAX_SPEED *{(speed > 0) - (speed < 0)};
    }
    //tmp = myserial.read();
    //tmp = map(tmp,0,255,0,359);
  }
  if(speed > 0){
    angle = 360 - caluculate_electorical_angle(OFFSET-60);//B = -60
  }else{
    angle = 360 - caluculate_electorical_angle(OFFSET-160);
  }
    
  if(angle >= 360) angle -= 360;
  if(angle <  0) angle += 360;
  direction = angle;
  Serial.println(speed);
  //Serial.print(",");
  //Serial.println(360 - caluculate_electorical_angle(tmp));
  //Serial.println(map(offset,0,255,0,359));

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

void square_wave_drive(int angle,int motor_speed){
  static int offset = 0;
  if(motor_speed < 0){
    motor_speed *= -1;
  }
  switch (angle){
    case 0:
      fets(HA,LA,motor_speed,1);//HIGH
      fets(HB,LB,0,1);//LOW
      fets(HC,LC,0,0);//FREE
      break;
    case 60:
      fets(HA,LA,motor_speed,1);//HIGH
      fets(HB,LB,0,0);//FREE
      fets(HC,LC,0,1);//LOW
      break;
    case 120:
      fets(HA,LA,0,0);//FREE
      fets(HB,LB,motor_speed,1);//HIGH
      fets(HC,LC,0,1);//LOW
      break;
    case 180:
      fets(HA,LA,0,1);//LOW
      fets(HB,LB,motor_speed,1);//HIGH
      fets(HC,LC,0,0);//FREE
      break;
    case 240:
      fets(HA,LA,0,1);//LOW
      fets(HB,LB,0,0);//FREE
      fets(HC,LC,motor_speed,1);//HIGH
      break;
    case 300:
      fets(HA,LA,0,0);//FREE
      fets(HB,LB,0,1);//LOW
      fets(HC,LC,motor_speed,1);//HIGH
      break;
    default:
      fets(HA,LA,0,0);//FREE
      fets(HB,LB,0,0);//FREE
      fets(HC,LC,0,0);//FREE
      break;
  }
}