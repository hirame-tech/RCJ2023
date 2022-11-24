#include <Arduino.h>
#include <func.hpp>

#define GYRO_SERIAL Serial7 //checked
#define IR_SERIAL Serial2
#define CAM_SERIAL Serial3

#define TRANS_SENSOR null//Transparent sensor
#define REF_SENSOR null//reflection sensor


LEDPIN led_pins;

LINEPIN line_pins;

//prototype declaration


void setup() {
  //pin_setup();
  Serial.begin(115200);
  GYRO_SERIAL.begin(115200);
  //motor1.begin(115200);
  //motor2.begin(115200);
  //motor3.begin(115200);

  led_pins.line_state = 1;
  led_pins.cam_state = 1;
  led_pins.gyro_state = 1;
  led_pins.IR_state = 1;
  led_pins.gyro_L = 1;
  led_pins.gyro_R = 1;
  //pinMode(led_pins.line_state,OUTPUT);
  //pinMode(led_pins.cam_state,OUTPUT);
  //pinMode(led_pins.gyro_state,OUTPUT);
  //pinMode(led_pins.IR_state,OUTPUT);
  //pinMode(led_pins.gyro_L,OUTPUT);
  //pinMode(led_pins.gyro_R,OUTPUT);

}

void loop() {
  //variable definition
  static bool line_frag = 0;
  static int gyro_angle = 127;

  static float IR_angle;

  //LED process
  if(gyro_angle < 120){
    digitalWrite(led_pins.gyro_L,HIGH);
    digitalWrite(led_pins.gyro_R,LOW);
  }else if(gyro_angle > 134){
    digitalWrite(led_pins.gyro_L,LOW);
    digitalWrite(led_pins.gyro_R,HIGH);
  }else{
    digitalWrite(led_pins.gyro_L,HIGH);
    digitalWrite(led_pins.gyro_R,HIGH);
  }


  gyro_angle = get_gyro(&GYRO_SERIAL,led_pins.gyro_state);
  if(IR_angle <= PI/4){

  }else if(IR_angle >= PI*3/4){

  }
  delay(1);
  /*
  static int line_state[30];
  static int line_threshold = 500;
  
  static float IR_angle;
  static int IR_distance;

  line_frag = get_line(line_pins,line_state,line_threshold);
  gyro_angle = get_gyro(&GYRO_SERIAL);
  if(line_frag == 1){
    //add motor stop process

    //add evacuation process
    
  }else{
    get_IR(&IR_SERIAL,&IR_angle,&IR_distance);
    if(IR_distance != 0){
      //add chase ball process
    }else{
      //add move fixed position process
    }
  }
  */
}