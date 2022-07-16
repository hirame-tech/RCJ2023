#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define BRIGHTNESS 0
#define LED_PIN 9

Adafruit_NeoPixel strip(30, LED_PIN, NEO_GRB + NEO_KHZ800);

void get_linesensor(int *pins, int *a_pins, int *value);

int set_port[8] = {7, 8, 6, 10, 3, 2, 4, 5};
int read_port[2] = {A1, A0};

int sensor_value[30];  //読み込んだ値を格納

void set_pin() {
    int i;
    for (i = 0; i < 8; i++) {
        pinMode(set_port[j], OUTPUT);
    }
    pinMode(read_port[0], INPUT);  // 0~14
    pinMode(read_port[1], INPUT);  // 15~30
}

void setup() {
    Serial.begin(9600);
    strip.begin();
    strip.show();  // Turn OFF
    strip.setBrightness(BRIGHTNESS);
    set_pin();
}

void loop() {
    for (int i = 0; i < 30; i++) {
        strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    strip.show();

    get_linesensor(set_port, read_port, sensor_value);
    Serial.println((sensor_value[29]));
}

/*
* @brief get linesensor value
* @param pins digitalpin number
* @param a_pins analogpin number
* @param value return sensor value
*/
void get_linesensor(int *pins, int *a_pins, int *value) {
    uint8_t mux_channel[16][4] = {
        {0, 0, 0, 0},  // 0
        {1, 0, 0, 0},  // 1
        {0, 1, 0, 0},  // 2
        {1, 1, 0, 0},  // 3
        {0, 0, 1, 0},  // 4
        {1, 0, 1, 0},  // 5
        {0, 1, 1, 0},  // 6
        {1, 1, 1, 0},  // 7
        {0, 0, 0, 1},  // 8
        {1, 0, 0, 1},  // 9
        {0, 1, 0, 1},  // 10
        {1, 1, 0, 1},  // 11
        {0, 0, 1, 1},  // 12
        {1, 0, 1, 1},  // 13
        {0, 1, 1, 1},  // 14
        {1, 1, 1, 1}   // 15
    };

    int i, j;

    for (i = 0; i < 16; i++) {
        for (j = 0; j < 4; j++) {
            digitalWrite(set_port[j], mux_channel[i][j]);
        }
        value[i] = analogRead(a_pins[0]);
    }
    for (i = 16; i < 30; i++) {
        for (j = 0; j < 4; j++) {
            digitalWrite(set_port[j + 4], mux_channel[i - 16][j]);
        }
        value[i] = analogRead(a_pins[1]);
    }
}

/*
n PT
PIN0
  0 1
  1 2
  2 3
  中略
  14 15
  15 16
PIN1
  16 17
  17 18
  18 19
  19 20
  20 21
　中略
  29 30
 */