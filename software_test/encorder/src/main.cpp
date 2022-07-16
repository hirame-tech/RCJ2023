#include <Arduino.h>
#include <AS5048A.h>

AS5048A angleSensor(10, false);

int direction;
int max = 0;

void setup() {
  Serial.begin(9600);
  angleSensor.begin();
}

void loop() {
  float val = angleSensor.getRotationInDegrees();
  Serial.println(val);

}