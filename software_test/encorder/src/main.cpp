#include <Arduino.h>
#include <AS5048A.h>
#define CSPIN 10
#define HA 0
#define LA 3
#define HB 1
#define LB 4
#define HC 2
#define LC 5

AS5048A angleSensor(CSPIN, false);

int direction;
int max = 0;

void setup() {
  Serial.begin(9600);
  angleSensor.begin();
	

}

void loop() {
  static int max = 0;
  float val = angleSensor.getRotationInDegrees();
  Serial.println(val);

}