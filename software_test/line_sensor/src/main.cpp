#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

#define BRIGHTNESS 0
#define LED_PIN 9
#define N 15

Adafruit_NeoPixel strip(30, LED_PIN, NEO_GRB + NEO_KHZ800);

void get_linesensor(int *pins, int *a_pins, int *value);

int set_port[8] = {7, 8, 6, 10, 3, 2, 4, 5};
int read_port[2] = {A1, A0};

int sensor_value[30];
int old_value[30];
void set_pin() {
    int i;
    for (i = 0; i < 8; i++) {
        pinMode(set_port[i], OUTPUT);
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
    Serial.println((sensor_value[N]));  //簡易平滑化
}

void get_linesensor(int *pins, int *a_pins, int *value) {
    /*int i = N;
    for (int num = 4; num < 8; num++) {
        if (bitRead(i, num) == 1) {
            digitalWrite(pins[num], HIGH);
        } else {
            digitalWrite(pins[num], LOW);
        }
    }

    value[i] = analogRead(a_pins[1]);*/
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

    int i;
    for (i = 4; i < 8; i++) {
        digitalWrite(set_port[i], mux_channel[N][i]);
    }
    value[N] = analogRead(a_pins[1]);
}

//スケッチの動作確認済み
//ボード上での動作についてはわからん

/*
0 PT1

15 PT16

 0 PT18
 1 PT19
 2 PT20

 6 24
12 30
13 

15 27



*/

void test(void) {
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

    int set_port[8] = {7, 8, 6, 10, 3, 2, 4, 5};
    int read_port[2] = {A1, A0};
    int i;
    for (i = 0; i < 4; i++) {
        digitalWrite(set_port[i], mux_channel[N][i]);
    }
}