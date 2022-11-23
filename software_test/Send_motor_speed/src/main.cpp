#include <Arduino.h>
//70~
void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  pinMode(A11,INPUT);
}

void loop() {
  int val = map(analogRead(A11),0,1023,0,255);
  Serial.println(val);
  Serial2.write(val);
  delay(1);
}