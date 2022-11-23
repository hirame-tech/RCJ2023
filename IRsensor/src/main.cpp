#include <Arduino.h>

#define CYCLE_TIME 833
#define SENSOR_NUMBER 16
#define COEFFICIENT 2

int pin[]={
  2,4,12,6,
  8,9,7,A5,
  A4,A3,A2,A1,
  A0,13, 5,10
  };


//prototype declaration
void get_IR(int *pins,int *datas);
int Ball_angle(int *datas);
int Ball_distance(int *datas);

void setup() {
  for (int i = 0; i < SENSOR_NUMBER; i++){
    pinMode(pin[i],INPUT);
  }
  Serial.begin(9600);
  Serial1.begin(115200);
}

void loop() {
  int sensor_data[SENSOR_NUMBER],distance,angle;
  uint8_t send_data;
  get_IR(pin,sensor_data);
  distance = Ball_distance(sensor_data) & 0x0F;
  angle = Ball_angle(sensor_data) & 0xF;
  send_data = ((distance << 4) + angle) & 0xFF;
  /*
  for (int i = 0; i < SENSOR_NUMBER; i++){
    Serial.print(sensor_data[i]);
    Serial.print(" : ");
  }
  */
  //Serial.println();
  if(distance != 0){
    Serial.println(angle);
  }else{
    Serial.println("ball is not found.");
  }
}

void get_IR(int *pins,int *datas){
  //初期化
  for (int i = 0; i < SENSOR_NUMBER; i++){
    datas[i] = 0;
  }
  //計測
  const unsigned long start_time= micros();
  do{
    for (int i = 0; i < SENSOR_NUMBER; i++){
      if((i < 5) || (11 < i)){
        datas[i] += (digitalRead(pins[i]) == LOW)*COEFFICIENT;
      }
      
    }
  }while ((micros() - start_time) < CYCLE_TIME);
}

/*
* @brief get angle
* @return 0~15
*/
int Ball_angle(int *datas){
  int max,index;
  max = 0;
  index = 0;
  for (int i = 0; i < SENSOR_NUMBER; i++){
    if(datas[i] > max){
      max = datas[i];
      index = i;
    }
  }

  return index;  
}

/*
* @brief get distance
* @return 0~15
*/
int Ball_distance(int *datas){
  int distance = 0;
  for (int i = 0; i < SENSOR_NUMBER; i++){
    distance += (datas[i] > 0);
  }
  if(distance == 1){
    distance = 2;
  }
  if(distance != 0){
    distance -= 1;
  }
  return distance;  
}