//とりあえずディレクトリを作ってファイルを入れるテスト
//ここだとビルド時に影響しないか不安だから適当に動かしてもらっておｋ
//一応プルのテスト
#include<Arduino.h>

int val = [30]

const int censorPIN = 13  //任意のポート番号

void setup(){
  pinMode(censorPIN,INPUT);
  Serial.begin(115200);
  
}

void loop(){
  val[0] = analogRead(censorPIN);
  Serial.println(val[0]);
  delay(100);

}
