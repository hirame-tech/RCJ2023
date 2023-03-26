#include <Arduino.h>

void setup() {
  Serial8.begin(115200);
  Serial.begin(9600);
}

void loop() {
  int angleb;//angle of blue    0~7:angle(center:0) 8:can't detect
  int angley;//angle of yellow
  char lr;// x = can't detect l = left r = right
  char by;// x = can't detect m = center b = blue y = yellow
  Serial8.write(123);
  if (Serial8.available() > 0) {
    String input = Serial8.readStringUntil('\n');
    //int input = Serial8.read();
    Serial.println(input);
    //angleb = input[0] - '0';
    //angley = input[1] - '0';
    //lr = input[2];
    //by= input[3];
    
  }
}