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
  //SERIAL.begin(115200);
  //motor1.begin(115200);
  //motor2.begin(115200);
  //motor3.begin(115200);

led_pins.line_state = 1;
led_pins.cam_state = 1;
led_pins.gyro_state = 1;
led_pins.IR_state = 1;
led_pins.gyro_L = 1;
led_pins.gyro_R = 1;

}

void loop() {
  //variable definition
  static int gyro_angle = 127;

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


  Serial.println(get_gyro(&GYRO_SERIAL,led_pins.gyro_state));
  
  delay(1);
  /*
  static bool line_frag = 0;
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