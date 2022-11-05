#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(A0,INPUT);
}

void loop() {
  int val = map(analogRead(A0),0,1023,0,200);
  Serial.println(val);
  Serial1.write(val);
}