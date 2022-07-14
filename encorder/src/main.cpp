#include <Arduino.h>
#include<SPI.h>

//prototype declaration
int get_encorder(int SSpin);

int direction;
int max = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  //pinMode(4,OUTPUT);
  digitalWrite(SS,HIGH);//SS = 10
}

void loop() {
  direction = get_encorder(SS) - 8224;
  direction = map(direction,0,8159,0,99);
  if(max < direction){
    max = direction;
  }
  Serial.println(direction);

}

int get_encorder(int SSpin){
  int data0,data1;
  digitalWrite(SSpin,LOW);//communication start
  SPI.transfer(0xFF);
  digitalWrite(SSpin,HIGH);

  digitalWrite(SSpin,LOW);
  data0 = SPI.transfer(0)&0xFF;
  digitalWrite(SSpin,HIGH);// communication end

  digitalWrite(SSpin,LOW);//communication start
  SPI.transfer(0xFF);
  digitalWrite(SSpin,HIGH);

  digitalWrite(SSpin,LOW);
  data1 = SPI.transfer(0)&0xFF;
  digitalWrite(SSpin,HIGH);// communication end

  //return ((data0 <<8) + data1) & 0x3FFF;//14bit
  return (((data0<<8) + data1) &0x3FFF);
}