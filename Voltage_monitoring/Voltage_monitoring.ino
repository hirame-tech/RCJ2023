#define SENSOR_PIN 1
#define FET_PIN 2
#define LED_PIN 0

#define TRESHOLD 100

void setup() {
  pinMode(SENSOR_PIN,INPUT);
  pinMode(FET_PIN,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
  static int flag_trigger = 0;
  int sensor_val = digitalRead(SENSOR_PIN);
  if(sensor_val == 0){// Low Voltage
    flag_trigger++;
  }else{
    flag_trigger = 0;
  }
  if(flag_trigger > 0){
    digitalWrite(FET_PIN,HIGH);//fet is negative
    digitalWrite(LED_PIN,LOW);
  }else{
    digitalWrite(FET_PIN,LOW);//fet is active
    digitalWrite(LED_PIN,HIGH);
  }
}
