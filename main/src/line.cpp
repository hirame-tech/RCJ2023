#include "line.hpp"
#include <Arduino.h>

LINE::LINE() {}

LINE::~LINE() {}

/**
 *
 */
void LINE::set_pin(int _ICpin1[], int _Apin1, int _ICpin2[], int _Apin2) {
    for (int i = 0; i < 4; i++) {
        ICpin1[i] = _ICpin1[i];
        ICpin2[i] = _ICpin2[i];
        pinMode(ICpin1[i], OUTPUT);
        pinMode(ICpin2[i], OUTPUT);
    }

    Apin1 = _Apin1;
    Apin2 = _Apin2;

    // INPUT_PULLDOWN depends on Teensy
    pinMode(Apin1, INPUT_PULLDOWN);
    pinMode(Apin2, INPUT_PULLDOWN);
}

/**
 * @brief convert degree to radian
 * @param degree
 * @return float
 */
float LINE::degree_to_rad(int degree) {
    if (degree < 0) {
        degree += 360;
    } else if (degree >= 360) {
        degree -= 360;
    }
    return ((float)degree * PI / 180);
}

int LINE::rad_to_degree(float rad) {
    int degree;
    degree = rad * (180 / PI);
    if (degree < 0) {
        degree += 360;
    } else if (degree >= 360) {
        degree -= 360;
    }
    return (degree);
}

bool LINE::get_line(int bool_value[], int threshold) {
    int sum = 0;
    uint8_t mux_channel[16][4] = {
        {0, 0, 0, 0}, // 0
        {1, 0, 0, 0}, // 1
        {0, 1, 0, 0}, // 2
        {1, 1, 0, 0}, // 3
        {0, 0, 1, 0}, // 4
        {1, 0, 1, 0}, // 5
        {0, 1, 1, 0}, // 6
        {1, 1, 1, 0}, // 7
        {0, 0, 0, 1}, // 8
        {1, 0, 0, 1}, // 9
        {0, 1, 0, 1}, // 10
        {1, 1, 0, 1}, // 11
        {0, 0, 1, 1}, // 12
        {1, 0, 1, 1}, // 13
        {0, 1, 1, 1}, // 14
        {1, 1, 1, 1}  // 15
    };

    for (int i = 0; i < 14; i++) {
        for (int j = 0; j < 4; j++) {
            digitalWrite(ICpin1[j], mux_channel[i][j]);
        }
        line_value[13 - i] = analogRead(Apin1);

        bool_value[13 - i] = line_value[13 - i] > threshold;

        // value[i] = analogRead(pins.Apin1) > threshold;
        sum += bool_value[13 - i];
        // sum += value[i];
    }
    for (int i = 14; i < 30; i++) {
        for (int j = 0; j < 4; j++) {
            digitalWrite(ICpin2[j], mux_channel[i - 14][j]);
        }
        line_value[43 - i] = analogRead(Apin2);

        bool_value[43 - i] = line_value[43 - i] > threshold;

        int tmp;
        sum += bool_value[43 - i];
    }

    // value[28] = 0;
    // value[29] = 0;
    // value[30] = 0;

    // for (int i = 0; i < 30; i++) {
    //     Serial.print(line_value[i]);
    //     Serial.print(",");
    // }
    // Serial.println();

    return (sum > 0);
}

// 反応しているセンサがある前提
void LINE::cal_line_direction(int data[], float *angle, float *distance) {
    // 座標の指定
    double x[30];
    double y[30];

    int angle_PT[30];

    int i, k;
    int s = 0;
    int tmp;
    int count1, count2;
    int lightcount = 0;
    int state[30];
    float testdegree;
    double a, b, c;
    // x[0]y[0]（各センサの座標をとる）
    // x[0]y[0] を PT1 に対応させている。

    // 2022-12-06:基準となるx軸を原点とPT1のセンサがなす直線にした。
    for (i = 0; i < 30; i++) {
        x[i] = 47 * cos(degree_to_rad(12 * i));
        y[i] = 47 * sin(degree_to_rad(12 * i));
    }

    // しきい値を満たすセンサの取得
    for (i = 0; i < 30; i++) {
        if (data[i]) {
            state[i] = 1;
            lightcount++;
        } else {
            state[i] = 0;
        }
    }

    // labeling
    int labelnum = 0;
    if (state[0] == 1) {
        labelnum = 1;
    }
    state[0] *= labelnum;
    for (int i = 1; i < 30; i++) {
        if ((state[i] - state[i - 1]) > 0) { // 微分
            labelnum++;
        }
        state[i] *= labelnum;
    }
    if ((state[0] * state[29]) > 0) { // 0と29が両方反応している
        for (int i = 29; i > -1; i--) {
            if (state[i] != 0) {
                state[i] = 1;
            } else {
                labelnum--;
                break;
            }
        }
    }

    // state配列の確認
    for (int i = 0; i < 30; i++) {
        Serial.print(state[i]);
        Serial.print(",");
    }

    // 反応したブロックの数の確認
    Serial.print(labelnum);
    Serial.print(",");

    // Serial.println();

    if (labelnum > 1) {

        // 以下新判定メソッド

        // int state[30] = {0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
        // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        int tmp1_start;
        int tmp1_end;
        int tmp2_start;
        int tmp2_end;
        int i, s;
        int normalize_tester = 0;

        for (i = 0; i < 30; i++) {
            if (state[i] == 1) {
                tmp1_start = i;
                s = i + 1;
                while (state[s] == 1) {
                    s++;
                }
                s = s - 1;
                tmp1_end = s;

                count1 = (int)(tmp1_end - tmp1_start) / 2 + tmp1_start;

                break;
            }
        }

        for (i = tmp1_end + 1; i < 30; i++) {

            if (state[i] == 2) {
                tmp2_start = i;
                s = i + 1;
                while (state[s] == 2) {
                    s++;
                }
                s = s - 1;
                tmp2_end = s;

                count2 = (int)(tmp2_end - tmp2_start) / 2 + tmp2_start;

                break;
            }
        }

        // y = ax + b -> a -1 b
        // y = -1/a * x   tan(theta) = -1/a c = -1/a
        // theta = arctan(c);

        // 0除算の場合分け
        if (x[count1] == x[count2]) {
            return;
        }
        a = (y[count1] - y[count2]) / (x[count1] - x[count2]);
        b = y[count1] - a * x[count1];
        *distance = (fabs(b) / sqrt(a * a + 1));
        *angle = (float)atan2((x[count1] + x[count2]) / 2, (y[count1] + y[count2]) / 2);

        *angle -= PI / 2;
        if (*angle < 0) {
            *angle = 2 * PI - *angle;
        }
        if (*angle > 2 * PI) {
            *angle -= 2 * PI;
        }

        for (int i = 23; i < 30; i++) {
            normalize_tester += state[i];
        }
        if (normalize_tester != 0) {
            *angle = 2 * PI - *angle;
        }

        // 以下テスト
        // initialize angle_PT[30]
        // for (int i = 0; i < 30; i++) {
        //     angle_PT[i] = 12 * (i + 1);
        // }

        /*PT1 angle_PT[0] = 360
        PT2 angle_PT[1] = 348
            .
            .
            .
        PT30 angle_PT[29] = 12 */

        // testdegree =
        //     ((angle_PT[count2] - angle_PT[count1]) / 2) + angle_PT[count1];
        // *angle = degree_to_rad(testdegree);

        // Serial.print(*distance);
        // Serial.print(",");
        //Serial.print(rad_to_degree(*angle));
        // Serial.print(",");
        // Serial.print(count1);
        // Serial.print(",");
        // Serial.print(count2);
        // Serial.print(",");
        // Serial.print(testdegree);
        // Serial.print(",");
        //Serial.println();

    } else if (labelnum == 1) { // ここｱ正常
        int tmp1_start;
        int tmp1_end;
        int normalize_tester = 0;

        for (int i = 0; i < 30; i++) {
            if (state[i] == 1) {
                tmp1_start = i;
                s = i + 1;
                while (state[s] == 1) {
                    s++;
                }
                s = s - 1;
                tmp1_end = s;

                // 0と29が両方反応していた場合
                if (state[0] == 1 && state[29] == 1) {
                    s = 29;
                    while (state[s] == 1) {
                        s--;
                    }
                    tmp1_start = s + 1;

                    int tmp1_start_minus = 30 - tmp1_start;
                    int tmp1_mid = (int)(tmp1_start_minus + tmp1_end) / 2;
                    if (tmp1_start_minus < tmp1_mid) {
                        count1 = tmp1_end - tmp1_mid;
                    } else {
                        count1 = 30 + (tmp1_end - tmp1_mid);
                    }
                } else {
                    count1 = (int)(tmp1_end - tmp1_start) / 2 + tmp1_start;
                }

                break;
            }
        }
        if (count1 == 31 || count1 == 30) {
            count1 = 0;
        }

        *distance = 47;
        *angle = (float)atan2(x[count1], y[count1]);

        if (count1 == 30) {
            *angle = PI / 2;
        }

        //*angle += PI;
        *angle -= PI / 2;
        if (*angle < 0) {
            *angle = 2 * PI - *angle;
        }
        if (*angle > 2 * PI) {
            *angle -= 2 * PI;
        }

        for (int i = 23; i < 30; i++) {
            normalize_tester += state[i];
        }
        if (normalize_tester != 0) {
            *angle = 2 * PI - *angle;
        }

        //*angle = 2 * PI - *angle;

        // testdegree = angle_PT[count1];
        // *angle = degree_to_rad(testdegree);
        // Serial.print(*distance);
        // Serial.print(",");

        // Serial.print(rad_to_degree(*angle));
        // Serial.print(",");
        // Serial.print(count1);
        // Serial.print(",");
        // Serial.print(testdegree);
        // Serial.print(",");
        //Serial.println();
    } else {
        return;
    }
}