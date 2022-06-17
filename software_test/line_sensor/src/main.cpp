#include<Arduino.h>
#include<Adafruit_NeoPixel.h>

#define BRIGHTNESS 200
#define LED_PIN 9

Adafruit_NeoPixel strip(30, LED_PIN, NEO_GRB + NEO_KHZ800);

void get_linesensor(int *pins,int *a_pins,int*value);

int set_port[8] = {7,8,6,10,3,2,4,5};
int read_port[2] = {A1,A0};

int sensor_value[30];
int old_value[30];
void set_pin(){
    for(int i = 0;i < 4; i++){
        pinMode(set_port[i],OUTPUT);
    }
    pinMode(read_port[0],INPUT);//0~14
    pinMode(read_port[1],INPUT);//15~30
}
void setup(){
    Serial.begin(9600);
    strip.begin();           
    strip.show();            // Turn OFF
    strip.setBrightness(BRIGHTNESS);

}

void loop(){
    for(int i = 0; i < 30; i++){
        strip.setPixelColor(i, strip.Color(0,255,0));
    }
    strip.show();

    get_linesensor(set_port,read_port,sensor_value);
    Serial.println((sensor_value[2]+old_value[2])/2);//簡易平滑化
    old_value[2] = sensor_value[2];
    /*
    for(int i=0; i < 30;i++){
        censor_value[i] = analogRead(censor_port[i]);
        Serial.print("censor");
        Serial.println(censor_port[i]);
        Serial.print(" = ");
        Serial.println(censor_value[i]);
        Serial.print("\n");
        delay(100);
    }*/
}

void get_linesensor(int *pins,int *a_pins,int*value){
    int i = 2;
    for(int num=0; num < 4; num++){
        if(bitRead(i,num) == 1){
            digitalWrite(pins[num],HIGH);
        }else{
            digitalWrite(num,LOW);
        }
    }
    value[i] = analogRead(a_pins[0]);
}

//スケッチの動作確認済み
//ボード上での動作についてはわからん