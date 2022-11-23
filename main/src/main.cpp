#include <Arduino.h>
//#include <func.hpp>

#define SERIAL Serial1

//prototype declaration


void setup() {
  //pin_setup();
  Serial.begin(115200);
  SERIAL.begin(115200);
  //motor1.begin(115200);
  //motor2.begin(115200);
  //motor3.begin(115200);
}

void loop() {
  static int val = 0;
  Serial.print("hello world! :");
  SERIAL.write(172);
  if(SERIAL.available() > 0){
    val = SERIAL.read();
  }
  Serial.println(val);
}