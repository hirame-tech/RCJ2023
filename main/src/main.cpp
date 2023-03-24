#include <Arduino.h>
#include <math.h>
#include <Adafruit_NeoPixel.h>
#include <motor_dc.hpp>
#include <line.hpp>
#include <func.hpp>

//**user settings**
#define BRIGHTNESS 255
#define MOVE_SPEED 30 // MAX50
#define IR_r 7        // 適当
#define LINE_THRESHOLD 350
#define LINE_STOP_TIME 500

#define LINE_LED_PIN 10

Adafruit_NeoPixel line_led(30, LINE_LED_PIN, NEO_GRB + NEO_KHZ800);

#define GYRO_SERIAL Serial7 // checked
#define IR_SERIAL Serial3   // checked
#define CAM_SERIAL Serial3

#define MOTOR_A_SERIAL Serial4
#define MOTOR_B_SERIAL Serial5 //
#define MOTOR_C_SERIAL Serial2 //
#define MOTOR_D_SERIAL Serial6

MOTOR motor(&MOTOR_B_SERIAL, &MOTOR_C_SERIAL);
LINE line;

#define TRANS_SENSOR null // Transparent sensor
#define REF_SENSOR null   // reflection sensor

#define SWITCH_PIN 41

LEDPIN led_pins;

// prototype declaration

void setup()
{
  // pin_setup();
  Serial.begin(115200);
  GYRO_SERIAL.begin(115200);
  IR_SERIAL.begin(115200);

  MOTOR_B_SERIAL.begin(115200);
  MOTOR_C_SERIAL.begin(115200);

  motor.free();

  // line IO settings
  const int d_pin1[] = {5, 4, 6, 9};
  const int d_pin2[] = {12, 11, 30, 31};
  line.set_pin(d_pin1, A12, d_pin2, A13);

  pinMode(SWITCH_PIN, INPUT_PULLUP);

  // led_pins.line_state = 1;
  // led_pins.cam_state = 1;
  // led_pins.gyro_state = 1;
  // led_pins.IR_state = 1;
  // led_pins.gyro_L = 1;
  // led_pins.gyro_R = 1;
  // pinMode(led_pins.line_state,OUTPUT);
  // pinMode(led_pins.cam_state,OUTPUT);
  // pinMode(led_pins.gyro_state,OUTPUT);
  // pinMode(led_pins.IR_state,OUTPUT);
  // pinMode(led_pins.gyro_L,OUTPUT);
  // pinMode(led_pins.gyro_R,OUTPUT);

  line_led.begin();
  line_led.show(); // Turn OFF
  line_led.setBrightness(BRIGHTNESS);

  for (int i = 0; i < 30; i++)
  {
    line_led.setPixelColor(i, line_led.Color(0, 255, 0));
    line_led.show();
    delay(10);
  }
}

void loop()
{
  // variable definition
  static bool line_frag = 0;                  // line reaction flag
  static bool line_frag_old = 0;              // line reaction flag
  static int line_state[30];                  // binary data
  static int line_threshold = LINE_THRESHOLD; // 将来的な可変抵抗化に対応するための変数
  static float line_approach_angle;
  static float line_angle;
  static float line_depth;

  static int gyro_angle = 127; // gyro angle data
  static bool start_flag = 0;  // start toggle switch status

  static float IR_angle;
  static int IR_distance;
  static int start_time = 0; // what is this

  static float my_goal_angle;
  static float opponent_goal_angle;
  static int my_goal_distance;
  static int opponent_goal_distancce;

  // LED process
  /*
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
  */

  // light up line LEDs
  for (int i = 0; i < 30; i++)
  {
    line_led.setPixelColor(i, line_led.Color(0, 255, 0));
  }
  line_led.show();

  // get value of various sensors
  start_flag = !(digitalRead(SWITCH_PIN));
  gyro_angle = get_gyro(&GYRO_SERIAL, led_pins.gyro_state);
  //line_frag_old = line_frag;
  line_frag = line.get_line(line_state, line_threshold);
  
  get_IR(&IR_SERIAL, &IR_angle, &IR_distance);
  Serial.println(IR_distance);

  // set line approach angle
  //if ((line_frag - line_frag_old) > 0)
  //{
  //  line_approach_angle = line_angle;
  //  start_time = millis();
  //}
  //else if (((line_frag - line_frag_old) < 0) && ((millis() - start_time) > 1000))
  //{
  //  line_approach_angle = -1;
  //}

  // cal line
  if (line_frag == 1)
  {
    //line.cal_line_direction(line_state, &line_angle, &line_depth);
    // Serial.println(line_angle*180/PI);
  }
  else
  {
    // Serial.println("can't find white");
  }

  if (start_flag == 1)
  { // start
    if (line_frag == 1)
    {
      // escape line zone
      motor.move(0, 0, 127);
    }
    else
    {
      if (IR_distance != 0)
      {
        // adjust IR distance
        IR_distance = 16 - IR_distance;
        if (IR_r > IR_distance)
        {
          IR_distance = IR_r;
        }

        // chase ball process
        if (IR_angle < PI / 4){
          motor.move(2 * IR_angle, MOVE_SPEED, gyro_angle);
        }
        else if (IR_angle < PI)
        {
          motor.move(IR_angle + asin(IR_r / IR_distance), MOVE_SPEED, gyro_angle);
        }
        else if (IR_angle < 3 * PI / 2)
        {
          motor.move(IR_angle - asin(IR_r / IR_distance), MOVE_SPEED, gyro_angle);
        }
        else
        {
          motor.move(2 * IR_angle - 2 * PI, MOVE_SPEED, gyro_angle);
        }
      }
      else
      {
        // 定位置いれたいな
        motor.move(0, 0, gyro_angle); // 静止
      }
    }
    // motor.move(0,0,(PI + my_goal_angle)*127/PI);//goal向き方向修正テスト用
  }
  else
  {
    motor.move(0, 0, 127); // 静止
  }
}