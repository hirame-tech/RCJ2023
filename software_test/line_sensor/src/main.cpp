#include<Arduino.h>

int censor_port[29] = {
    //任意のポート番号を入力　値,値,値,・・・
};

int censor_value[29] = {};

void setup(){
    Serial.begin(9600);
}

void loop(){
    for(int i=0; i < 30;i++){
        censor_value[i] = analogRead(censor_port[i]);
        Serial.print("censor");
        Serial.println(censor_port[i]);
        Serial.print(" = ");
        Serial.println(censor_value[i]);
        Serial.print("\n");
        delay(100);
    }
}

//スケッチの動作確認済み
//ボード上での動作についてはわからん