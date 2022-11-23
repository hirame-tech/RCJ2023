#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <math.h>

#define BRIGHTNESS 50
#define LED_PIN 9

Adafruit_NeoPixel strip(30, LED_PIN, NEO_GRB + NEO_KHZ800);

void get_linesensor(int *pins, int *a_pins, int *value);

void get_rad(int data[], int threshold, float *angle, float *distance);

double degree_to_rad(int degree);

int set_port[8] = {7, 8, 6, 10, 3, 2, 4, 5};
int read_port[2] = {A1, A0};

int sensor_value[30];  //読み込んだ値を格納

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
    int threshold = 100;  // any value
    float angle, distance;

    for (int i = 0; i < 30; i++) {
        strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    strip.show();

    get_linesensor(set_port, read_port, sensor_value);
    /*for (int i = 0; i < 29; i++) {
        Serial.print(sensor_value[i]);
        Serial.print(",");
    }
    Serial.println(sensor_value[29]);*/
    Serial.println(sensor_value[0]);

    get_rad(sensor_value, threshold, &angle, &distance);
    /*Serial.print(angle);
    Serial.print(",");
    Serial.println(distance);*/


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

void get_rad(int data[], int threshold, float *angle, float *distance) {
    //座標の指定
    double x[30];
    double y[30];

    int i, k;
    int tmp;
    int count1, count2;
    for (i = 7, k = 0; i >= 0; i--, k++) {
        x[i] = 47 * cos(degree_to_rad(12 * k + 6));
        y[i] = 47 * sin(degree_to_rad(12 * k + 6));
    }
    for (i = 29, k = 8; i >= 8; i--, k++) {
        x[i] = 47 * cos(degree_to_rad(12 * k + 6));
        y[i] = 47 * sin(degree_to_rad(12 * k + 6));
    }

    //しきい値を満たすセンサの取得 *改善が必要
    for (i = 0; i < 30; i++) {
        if (data[i] > threshold) {
            count1 = i;
            tmp = data[i];
        }
        if (data[i] > tmp) {
            count2 = i;
        }
    }

    // y = ax + b -> a -1 b
    // y = -1/a * x   tan(theta) = -1/a c = -1/a
    // theta = arctan(c);
    double a, b, c;
    a = (y[count1] - y[count2]) / (x[count1] - x[count2]);
    b = y[count1] - a * x[count1];
    c = (-1) / a;

    *distance = (fabs(b) / sqrt(a * a + 1));
    *angle = tan(c);
}

double degree_to_rad(int degree) {
    double rad;

    rad = degree * (PI / 180);
    return rad;
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
