#include <Arduino.h>
#include <func.hpp>

Serial1 motor0;
Serial2 motor1;
Serial3 motor2;
Serial4 motor3;

//prototype declaration


void setup() {
  pin_setup();
  Serial.begin(115200);
  motor0 begin(115200);
  motor1.begin(115200);
  motor2.begin(115200);
  motor3.begin(115200);
}

void loop() {
  
}