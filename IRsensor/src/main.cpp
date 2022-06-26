#include <Arduino.h>

int pin[]={
  NULL,NULL,NULL,NULL,
  NULL,NULL,7,A5,
  A4,A3,A2,A1,
  A0,13, 5,10
  };


//prototype declaration
get_IR(int *pins,int *datas);

void setup() {
  for (int i = 0; i < 16; i++){
    pinMode(pin[i],INPUT);
  }
  Serial.begin(9600);
  Serial1.begin(115200);
}

void loop() {
  get_IR()
}