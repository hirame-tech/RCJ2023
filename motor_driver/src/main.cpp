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

#define OFFSET -260 //電気角//260
#define PI 3.1415926535


//#define DELAYTIME 2000
#define RPM 1000
#define VOLT 7.6
#define CAL_S(r,v) r/(400*v)*255
#define CAL_D(r)   10000000/(r*7) * 1.5

#define MOTOR_SPEED 200//CAL_S(RPM,VOLT)
#define DELAYTIME CAL_D(RPM)

int old;

const int pins[] = {HA,LA,HB,LB,HC,LC};

AS5048A angleSensor(CSPIN,false);

int direction;
int angle = 0;
int n;
int a[3];
float _sin[720];
int val = 0;
int val_sq = 0;
int motor_speed = 1;
//極対数　14

//prototype declaration

void fets(int H_pin,int L_pin,int duty,int mode);
int caluculate_electorical_angle(int offset);
void square_wave_drive(int angle,int speed,int direction);
void sin_wave_drive(int angle,int r[]);
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
  //LEDs
	pinMode(17,OUTPUT);
  //pinMode(16,OUTPUT);
  //pinMode(25,OUTPUT);
  //digitalWrite(17,HIGH);//OFF
  //digitalWrite(16,HIGH);//OFF
  //digitalWrite(25,HIGH);//OFF

  for(int i=0; i<720; i++){
    _sin[i] = sin(i * PI / 180);
  }

  //始動シーケンス1
  square_wave_drive(0,100,1);
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

  //static int angle = 0;

  static int old_angle = 0;
  static unsigned long time = micros();
  static unsigned long time_sq = micros();

  //矩形波駆動用
  /*
  if((micros() - time_sq) > (DELAYTIME)){
    val_sq += 60;
    time_sq = micros();
  }


  //正弦波駆動用
  
  if((micros() - time) > ((DELAYTIME) / 10)){
    time = micros();
    val += 6;
    val_sq = (val/60)*60 + 60;
  }
  */
  
  
  if(val >= 360) val -= 360;
  if(val <  0) val += 360;
  if(val_sq >= 360) val_sq -= 360;
  if(val_sq <  0) val_sq += 360;
  if(angle >= 360) angle -= 360;
  if(angle <  0) angle += 360;
  if(motor_speed >= 0){
    square_wave_drive(angle,abs(motor_speed),1);
  }else{
    square_wave_drive(angle,abs(motor_speed),-1);
  }
  //sin_wave_drive(val,a);

  n = micros() - loop_time; 
  loop_time = micros();
}

void loop1(){
  if(motor_speed >= 0){
    angle = caluculate_electorical_angle(OFFSET+35);
  }else{
    angle = caluculate_electorical_angle(OFFSET-35);
  }
  angle = caluculate_electorical_angle(OFFSET);
  if(myserial.available()){
    motor_speed = (myserial.read()-100)*2;
  }
  //angleSensor.begin();
  //Serial.print(val_sq);
  //Serial.print(",");
  //Serial.println(val);
  //Serial.print(a[0]);
    //Serial.print(",");
    //Serial.print(a[1]);
    //Serial.print(",");
    //Serial.print(a[2]);
    
  //Serial.print(val_sq);
  //Serial.print(",");
    //Serial.print(abs(val - encoder));
  Serial.print(angle);
  Serial.print(",");
  Serial.println(motor_speed);
}

int caluculate_electorical_angle(int offset){
  static float old_val;
  float tmp = angleSensor.getRotationInDegrees();
  String error = angleSensor.getErrors();

  if(error == ""){
    float val = tmp;
    //caluculate electorical angle
    float angle_electorical = val * 7  + offset;
    angle_electorical = angle_electorical - 360*((int)angle_electorical / 360);

    if(angle_electorical < 0){
      angle_electorical += 360;
    }else if(angle_electorical > 360){
      angle_electorical -= 360;
    }

    int angle_electorical_int = ((int)angle_electorical / 60) * 60;
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
  //case -1://connect low
  //  analogWrite(H_pin,0);
  //  analogWrite(L_pin,255-duty);
  //  break;
  default:
    Serial.printf("fets : exception of (int)mode\n");
    break;
  }
}

void square_wave_drive(int angle,int speed,int direction){
  static int offset = 0;
  switch (direction){
    case 1:
      switch (angle){
        case 0:
          fets(HA,LA,speed,1);//0//60
          fets(HB,LB,0,1);
          fets(HC,LC,0,0);
          break;
        case 300:
          fets(HA,LA,speed,1);//60//120
          fets(HB,LB,0,0);
          fets(HC,LC,0,1);
          break;
        case 240:
          fets(HA,LA,speed,0);//120//180
          fets(HB,LB,speed,1);
          fets(HC,LC,0,1);
          break;
        case 180:
          fets(HA,LA,0,1);//180//240
          fets(HB,LB,speed,1);
          fets(HC,LC,speed,0);
          break;
        case 120:
          fets(HA,LA,0,1);//240//300
          fets(HB,LB,speed,0);
          fets(HC,LC,speed,1);
          break;
        case 60:
          fets(HA,LA,speed,0);//300//0
          fets(HB,LB,0,1);
          fets(HC,LC,speed,1);
          break;
        default:
          fets(HA,LA,speed,0);
          fets(HB,LB,speed,0);
          fets(HC,LC,speed,0);
          break;
      }
    case -1:
      switch (angle){
        case 0:
          fets(HA,LA,0,1);//0//60
          fets(HB,LB,speed,1);
          fets(HC,LC,speed,0);
          break;
        case 60:
          fets(HA,LA,0,1);//60//120
          fets(HB,LB,speed,0);
          fets(HC,LC,speed,1);
          break;
        case 120:
          fets(HA,LA,speed,0);//120//180
          fets(HB,LB,0,1);
          fets(HC,LC,speed,1);
          break;
        case 180:
          fets(HA,LA,speed,1);//180//240
          fets(HB,LB,0,1);
          fets(HC,LC,speed,0);
          break;
        case 240:
          fets(HA,LA,speed,1);//240//300
          fets(HB,LB,speed,0);
          fets(HC,LC,0,1);
          break;
        case 300:
          fets(HA,LA,speed,0);//300//0
          fets(HB,LB,speed,1);
          fets(HC,LC,0,1);
          break;
        default:
          fets(HA,LA,speed,0);
          fets(HB,LB,speed,0);
          fets(HC,LC,speed,0);
          break;
      }
  }

}

void sin_wave_drive(int angle,int r[]){
  int speed_A = (int)( MOTOR_SPEED*_sin[angle]/2 + MOTOR_SPEED/2);
  int speed_B = (int)( MOTOR_SPEED*_sin[angle+120]/2 + MOTOR_SPEED/2);
  int speed_C = (int)( MOTOR_SPEED*_sin[angle+240]/2 + MOTOR_SPEED/2);
  r[0] = abs(speed_A) * sign(speed_A);
  r[1] = abs(speed_B) * sign(speed_B);
  r[2] = abs(speed_C) * sign(speed_C);
  fets(HA,LA,abs(speed_A),sign(speed_A));
  fets(HB,LB,abs(speed_B),sign(speed_B));
  fets(HC,LC,abs(speed_C),sign(speed_C));
}

int sign(int n){
  return (n>0)-(n<0);
}