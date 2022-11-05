#include <Arduino.h>
#include <AS5048A.h>
#define CSPIN D6
#define HA D0
#define HB D1
#define HC D2
#define LA D3
#define LB D4
#define LC D5

#define OFFSET 75 //電気角
#define PI 3.1415926535
#define MOTOR_SPEED 60

#define DELAYTIME 1
int old;

const int pins[] = {HA,LA,HB,LB,HC,LC};

AS5048A angleSensor(CSPIN,false);

int direction;
int a[3];
//極対数　14

//prototype declaration

void fets(int H_pin,int L_pin,int duty,int mode);
int caluculate_electorical_angle(int offset);
void square_wave_drive(int angle);
void sin_wave_drive(int angle,int r[]);
int sign(int number);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  angleSensor.begin();

  pinMode(D7,INPUT);//何これ
  analogWriteFreq(25000);//10kHz
  for (int i = 0; i < 6; i++){
    pinMode(pins[i],OUTPUT);
    //digitalWrite(pins[i],LOW);
  }
  fets(HA,LA,MOTOR_SPEED,0);
  fets(HB,LB,MOTOR_SPEED,0);
  fets(HC,LC,MOTOR_SPEED,0);
  //LEDs
	pinMode(17,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(25,OUTPUT);
  digitalWrite(17,HIGH);//OFF
  digitalWrite(16,HIGH);//OFF
  digitalWrite(25,HIGH);//OFF
  delay(2000);

  //始動シーケンス1
  square_wave_drive(0);
  delay(100);
  square_wave_drive(-1);
  delay(10);

}

void loop() {
  static int angle = 0;
  static int val = 0;
  
  static unsigned long time = micros();
  //矩形波駆動用
  /*
  if((micros() - time) > (DELAYTIME*1000 + 100)){
    val += 60;
    time = micros();
  }
  */
  angle = caluculate_electorical_angle(OFFSET) + 10;
  //正弦波駆動用
  if((micros() - time) > ((DELAYTIME*1000) / 60)){
    val ++;
    //Serial.print(a[0]);
    //Serial.print(",");
    //Serial.print(a[1]);
    //Serial.print(",");
    //Serial.print(a[2]);
    
    //Serial.print(val);
    //Serial.print(",");
    //Serial.print(abs(val - encoder));
    //Serial.print(",");
    //Serial.print(",");
    //Serial.println(angle);
    
    time = micros();
  }

  if(val >= 360) val -= 360;
  if(val <  0) val += 360;
  if(angle >= 360) angle -= 360;
  if(angle <  0) angle += 360;
  
  sin_wave_drive(val,a);
  //fets(HA,LA,20,1);
  //fets(HB,LB,20,-1);
  //fets(HC,LC,0,0);


  //delayMicroseconds(1000);
  //fets(HA,LA,MOTOR_SPEED,1);
  //fets(HB,LB,MOTOR_SPEED,1);
  //fets(HC,LC,MOTOR_SPEED,1);
  //delay(10);
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

    int angle_electorical_int = 360 - (int)angle_electorical;
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
  case -1://connect low
    analogWrite(H_pin,0);
    analogWrite(L_pin,255-duty);
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
      fets(HB,LB,MOTOR_SPEED,-1);
      fets(HC,LC,MOTOR_SPEED,0);
      Serial.println(caluculate_electorical_angle(offset));
      break;
    case 60:
      fets(HA,LA,MOTOR_SPEED,1);//60//120
      fets(HB,LB,MOTOR_SPEED,0);
      fets(HC,LC,MOTOR_SPEED,-1);
      break;
    case 120:
      fets(HA,LA,MOTOR_SPEED,0);//120//180
      fets(HB,LB,MOTOR_SPEED,1);
      fets(HC,LC,MOTOR_SPEED,-1);
      break;
    case 180:
      fets(HA,LA,MOTOR_SPEED,-1);//180//240
      fets(HB,LB,MOTOR_SPEED,1);
      fets(HC,LC,MOTOR_SPEED,0);
      break;
    case 240:
      fets(HA,LA,MOTOR_SPEED,-1);//240//300
      fets(HB,LB,MOTOR_SPEED,0);
      fets(HC,LC,MOTOR_SPEED,1);
      break;
    case 300:
      fets(HA,LA,MOTOR_SPEED,0);//300//0
      fets(HB,LB,MOTOR_SPEED,-1);
      fets(HC,LC,MOTOR_SPEED,1);
      break;
    default:
      fets(HA,LA,MOTOR_SPEED,0);//300//0
      fets(HB,LB,MOTOR_SPEED,0);
      fets(HC,LC,MOTOR_SPEED,0);
      break;
  }
}

void sin_wave_drive(int angle,int r[]){
  float rad_angle = angle * PI / 180;
  int speed_A = (int)( MOTOR_SPEED*sin(rad_angle)/2 + MOTOR_SPEED/2);
  int speed_B = (int)( MOTOR_SPEED*sin(rad_angle  +(PI * 2/ 3))/2 + MOTOR_SPEED/2);
  int speed_C = (int)( MOTOR_SPEED*sin(rad_angle  +(PI * 4/3))/2 + MOTOR_SPEED/2);
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