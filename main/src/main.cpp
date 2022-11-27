#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <motor.hpp>
#include <func.hpp>

#define BRIGHTNESS 50
#define LINE_LED_PIN 10

Adafruit_NeoPixel line_led(30, LINE_LED_PIN, NEO_GRB + NEO_KHZ800);

#define GYRO_SERIAL Serial7 //checked
#define IR_SERIAL Serial2
#define CAM_SERIAL Serial3

#define MOTOR_A_SERIAL Serial4
#define MOTOR_B_SERIAL Serial5
#define MOTOR_C_SERIAL Serial2
#define MOTOR_D_SERIAL Serial6

MOTOR motor(&MOTOR_A_SERIAL,&MOTOR_B_SERIAL,&MOTOR_C_SERIAL,&MOTOR_D_SERIAL);

#define TRANS_SENSOR null//Transparent sensor
#define REF_SENSOR null//reflection sensor

#define SWITCH_PIN 3


LEDPIN led_pins;

LINEPIN line_pins;

//prototype declaration


void setup() {
  //pin_setup();
  Serial.begin(115200);
  GYRO_SERIAL.begin(115200);

  MOTOR_A_SERIAL.begin(9600);
  MOTOR_B_SERIAL.begin(9600);
  MOTOR_C_SERIAL.begin(9600);
  MOTOR_D_SERIAL.begin(9600);
  motor.free();

  led_pins.line_state = 1;
  led_pins.cam_state = 1;
  led_pins.gyro_state = 1;
  led_pins.IR_state = 1;
  led_pins.gyro_L = 1;
  led_pins.gyro_R = 1;
  pinMode(SWITCH_PIN,INPUT_PULLUP);
  //pinMode(led_pins.line_state,OUTPUT);
  //pinMode(led_pins.cam_state,OUTPUT);
  //pinMode(led_pins.gyro_state,OUTPUT);
  //pinMode(led_pins.IR_state,OUTPUT);
  //pinMode(led_pins.gyro_L,OUTPUT);
  //pinMode(led_pins.gyro_R,OUTPUT);



    line_led.begin();
    line_led.show();  // Turn OFF
    line_led.setBrightness(BRIGHTNESS);

}

void loop() {
  //variable definition
  static bool line_frag = 0;
  static int gyro_angle = 127;
  static bool start_flag = 0;

  static float IR_angle;

  start_flag = digitalRead(SWITCH_PIN);

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

  for (int i = 0; i < 30; i++) {
      line_led.setPixelColor(i, line_led.Color(0, 255, 0));
  }
    line_led.show();

  gyro_angle = get_gyro(&GYRO_SERIAL,led_pins.gyro_state);
  if(start_flag == 0){//start
    motor.move(0,0,gyro_angle);//方向修正のみ
    if(IR_angle <= PI/4){

    }else if(IR_angle >= PI*3/4){

    }
  }else{
    motor.move(0,0,127);//静止
  }

  //Serial.println(gyro_angle);


  //delay(1);
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